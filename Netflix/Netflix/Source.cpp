#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <cstdio>

using namespace std;

random_device rng;
mt19937 run(rng());
uniform_int_distribution<mt19937::result_type> duration(20, 60);
uniform_int_distribution<mt19937::result_type> satisfaction(1, 10);

constexpr auto MAX_MINUTE = 180;
constexpr auto NUMBER_OF_MOVIES = 10;
constexpr auto MAX_ARRAY_SIZE = 1024;
//180 dakika 20'ser dakikadan en fazla 9 olabilir
constexpr auto MAX_NUMBER_OF_MOVIES = 9;
//180 dakika 60'ar dakikadan en az 3 olabilir
constexpr auto MIN_NUMBER_OF_MOVIES = 3;
//ustteki nedenlerden dolayi 967 ihtimal olabilir
constexpr auto ARRAY_SIZE = 967;

//kombinasyonlari dosyaya yaziyor
void comb(const int* arr, int* data, const int start, const int end, const int row, const int dataSize, fstream& combFile) {
	if (dataSize == row) {
		for (auto i{ 0 }; i < dataSize; ++i) {
			combFile << data[i];
		}
		combFile << "\n";
	}

	for (auto i{ start }; i < end && end + row >= dataSize + i; ++i) {
		data[row] = arr[i];
		comb(arr, data, i + 1, end, row + 1, dataSize, combFile);
	}
}

void swap(double* ptr, const int index) {
	double temp = ptr[index];
	ptr[index] = ptr[index + 1];
	ptr[index + 1] = temp;
}

void swap(int* ptr) {
	int temp[MAX_NUMBER_OF_MOVIES];
	memset(temp, 0, sizeof(int) * MAX_NUMBER_OF_MOVIES);

	for (auto i{ 0 }; i < MAX_NUMBER_OF_MOVIES; ++i) {
		temp[i] = ptr[i];
		ptr[i] = (ptr + MAX_NUMBER_OF_MOVIES)[i];
		(ptr + MAX_NUMBER_OF_MOVIES)[i] = temp[i];
	}
}

int main() {
	//dakikayi, puani ve puan/dakika olarak verimliligi tutacak dizilerin tanimlanmasi
	double minutes[NUMBER_OF_MOVIES];
	double points[NUMBER_OF_MOVIES];
	double efficiency[NUMBER_OF_MOVIES];

	//ALTERNATIF
	//double* minutes = new double[NUMBER_OF_MOVIES];
	//double* points = new double[NUMBER_OF_MOVIES];
	//double* efficiency = new double[NUMBER_OF_MOVIES];

	//dizilerin rastgele degerlerle ilklendirilmesi
	for (auto i{ 0 }; i < NUMBER_OF_MOVIES; ++i) {
		minutes[i] = duration(run);
		points[i] = satisfaction(run);
		efficiency[i] = static_cast<double>(points[i] / minutes[i]);
	}

	//dizilerin verimlilik sirasina gore siralanmasi, buyukten kucuge
	for (auto i{ 0 }; i < NUMBER_OF_MOVIES - 1; ++i) {
		for (auto j{ 0 }; j < NUMBER_OF_MOVIES - i - 1; ++j) {
			if (efficiency[j] < efficiency[j + 1]) {
				swap(efficiency, j);
				swap(minutes, j);
				swap(points, j);
			}
		}
	}

	//tablo seklinde random filmleri bastirir
	cout << "Filmler: " << endl;
	cout << "Numara   Dakika   Puan" << endl;
	for (auto i{ 0 }; i < NUMBER_OF_MOVIES; ++i) {
		//cout << left << setw(9) << i + 1 << setw(9) << minutes[i] << setw(4) << points[i] << endl;
		cout << right << setw(6) << i + 1 << setw(9) << minutes[i] << right << setw(7) << points[i] << endl;
	}
	cout << endl;

	//greedy ile hesaplanan toplam dakikayi tutar
	auto totalMinute{ 0 };
	//greedy ile hesaplanan toplam puani tutar
	auto totalPoints{ 0 };
	//greedy icinde minutes dizisinde gezilen indexi tutar
	auto indexMin{ 0 };
	//greedy dizisinde kac eleman oldugunu tutar
	auto indexGreedy{ 0 };
	double* greedy = new double[NUMBER_OF_MOVIES];

	//sureyi asmadýgý surece en verimli olani ekliyor
	while (true) {
		//ifte toplam sureye eklenince sinir olan 180 dakikayi geciyor mu diye bakiyor
		if (minutes[indexMin] + totalMinute <= MAX_MINUTE) {
			//gecmiyorsa sureyi ve puani ekliyor
			totalMinute += minutes[indexMin];
			totalPoints += points[indexMin];
			//greedy icinde kacinci filmlerin izlenmesi gerektigi tutuluyor
			greedy[indexGreedy++] = indexMin++;
		}
		else {
			//minutes dizisinin sonuna gelindiyse
			if (NUMBER_OF_MOVIES == ++indexMin) {
				break;
			}
		}
	}

	//greedy ile hesaplanan filmleri yazdiriyor
	cout << "Greedy ile izlenmesi gereken filmler: ";
	for (auto i{ 0 }; i < indexGreedy - 1; ++i) {		//boyle yapmazsam sonda virgul kaliyordu
		cout << greedy[i] + 1 << ", ";
	}
	cout << greedy[indexGreedy - 1] + 1;
	cout << endl << "Toplam sure: " << totalMinute << endl <<
		"Toplam puan: " << totalPoints << endl << endl;

	//ileride kullanmak icin yedek aliyorum
	//cunku yedegini aldiklarim degistirilecek
	auto greedyTotalMinute{ totalMinute };
	auto greedyTotalPoints{ totalPoints };

	//kombinasyon yazmak icin dosya aciliyor
	fstream combFile;
	combFile.open("combs.txt", ios::out);
	//dosya acildi mi diye bakiyor
	if (!combFile) {
		cout << "Dosya yazmak icin acilamadi." << endl;
		combFile.close();
		return -1;
	}

	//array olusturup icini sifir ile dolduruyor
	int arr[NUMBER_OF_MOVIES];
	for (auto i{ 0 }; i < NUMBER_OF_MOVIES; ++i) {
		arr[i] = i;
	}

	//kombinasyonlari dosyaya yaziyor
	for (auto i{ MIN_NUMBER_OF_MOVIES }; i < NUMBER_OF_MOVIES; ++i) {
		int data[NUMBER_OF_MOVIES];
		comb(arr, data, 0, NUMBER_OF_MOVIES, 0, i, combFile);
	}

	//yazma bitince kapatip tekrar okumak icin aciyor
	combFile.close();
	combFile.open("combs.txt", ios::in);
	//dosya acildi mi diye bakiyor
	if (!combFile) {
		cout << "Dosya okumak icin acilamadi." << endl;
		combFile.close();
		return -1;
	}

	//toplam degerleri tutacak array'ler
	double minutesArr[MAX_ARRAY_SIZE];
	double pointsArr[MAX_ARRAY_SIZE];
	double efficiencyArr[MAX_ARRAY_SIZE];
	//array'lerin doldurulmasi
	//efficiency points/minutes olarak tanimlanmis durumda
	//bos kalinca cop degerlerden 1 gelebiliyor
	//normalde gelen degerler 1'den daha kucuk
	//bu da siralamayi bozuyor
	//bu sekilde dusuk bir deger geliyor ve siralamaya etki etmiyor
	memset(minutesArr, 1, sizeof(double) * MAX_ARRAY_SIZE);
	memset(pointsArr, 0.01, sizeof(double) * MAX_ARRAY_SIZE);
	memset(efficiencyArr, 0, sizeof(double) * MAX_ARRAY_SIZE);

	//tum indexleri tutacak array'ler
	int movies[MAX_ARRAY_SIZE][MAX_NUMBER_OF_MOVIES];
	//icinin tamami dolmayacak
	//sifir gorene kadar hesap yapacagim
	//o yuzden sifir dolduruyorum
	memset(movies, 0, sizeof(int) * MAX_ARRAY_SIZE * MAX_NUMBER_OF_MOVIES);

	//toplam degiskenlerini sifirliyor
	totalMinute = 0;
	totalPoints = 0;

	//satir, sutun, sayac ve okunacak karakter tanimlaniyor
	auto row{ 0 };
	auto column{ 0 };
	char c;

	//tek karakter alip c degiskenine atiyor
	while (combFile.get(c)) {
		//satir sonu ise
		if ('\n' == c) {
			//toplam degerleri array'e atiyor
			if (MAX_MINUTE >= totalMinute) {
				minutesArr[row] = totalMinute;
				pointsArr[row] = totalPoints;
				//array'de diger satira geciyor
				row++;
			}

			//toplam degiskenlerini sifirliyor
			totalMinute = 0;
			totalPoints = 0;
			//sutunu sifirliyor
			column = 0;
			continue;
		}

		//indexten film listesindeki degerleri alip
		//toplam degiskenine ekliyor
		totalMinute += minutes[c - '0'];
		totalPoints += points[c - '0'];
		//tum ihtimalleri tutan dizide indexi kaydediyor
		//sifir gorunce durmasi gerektigi icin 1 ekliyorum
		movies[row][column] = c + 1 - '0';
		//dizide ilerlemek icin sutun degiskenini artiriyor
		column++;
	}

	//verimlilik dizisinin degerlerini hesapliyor
	for (auto i{ 0 }; i < MAX_ARRAY_SIZE; ++i) {
		efficiencyArr[i] = pointsArr[i] / minutesArr[i];
	}

	//tum ihtimallerin oldugu arrayleri verimlilige gore siraliyor
	for (auto i{ 0 }; i < MAX_ARRAY_SIZE - 1; ++i) {
		for (auto j{ 0 }; j < MAX_ARRAY_SIZE - i - 1; ++j) {
			if (efficiencyArr[j] < efficiencyArr[j + 1]) {
				swap(minutesArr, j);
				swap(pointsArr, j);
				swap(efficiencyArr, j);
				swap(movies[j]);
			}
		}
	}

	//tum ihtimallerin oldugu arrayleri puana gore siraliyor
	for (auto i{ 0 }; i < MAX_ARRAY_SIZE - 1; ++i) {
		for (auto j{ 0 }; j < MAX_ARRAY_SIZE - i - 1; ++j) {
			if (pointsArr[j] < pointsArr[j + 1]) {
				swap(minutesArr, j);
				swap(pointsArr, j);
				swap(efficiencyArr, j);
				swap(movies[j]);
			}
		}
	}

	//brute force ile hesaplanani yazdiriyor
	//sonda virgul birakmasin diye biraz degisik yazdiriyor
	auto indexBrute{ 0 };
	cout << "Brute force ile izlenemsi gerekenler: ";
	for (; indexBrute < MAX_NUMBER_OF_MOVIES; ++indexBrute) {
		if (2 < indexBrute && 0 == movies[0][indexBrute + 1]) {
			break;
		}
		cout << movies[0][indexBrute] << ", ";
	}
	cout << movies[0][indexBrute];	//boyle yapmazsam sonda virgul kaliyordu
	cout << endl << "Toplam sure: " << minutesArr[0] << endl << "Toplam puan: " << pointsArr[0] << endl;

	//brute ile greedy arasindakileri liste olarak veriyor
	//listedeki ilk eleman brute, sonuncu ise greedy oluyor
	//tek eleman varsa greedy en verimli yolu bulmus demektir
	auto counter{ 0 };
	cout << endl << "Greedy'ye kadar olan en verimli liste: " << endl;
	while (!(greedyTotalMinute == minutesArr[counter] && greedyTotalPoints == pointsArr[counter])) {
		cout << "Toplam dk: " << minutesArr[counter] << "   Toplam puan: " << pointsArr[counter] << "   Filmler: ";
		for (indexBrute=1; 0 != movies[counter][indexBrute]; ++indexBrute) {
			cout << movies[counter][indexBrute -1]<<", ";
		}
		cout << movies[counter][indexBrute - 1]<<endl;
		counter++;
	}
	cout << "Toplam dk: " << minutesArr[counter] << "   Toplam puan: " << pointsArr[counter] << "   Filmler: ";
	for (indexBrute = 1; 0 != movies[counter][indexBrute]; ++indexBrute) {
		cout << movies[counter][indexBrute - 1] << ", ";
	}
	cout << movies[counter][indexBrute - 1]<<endl;

	//bilgilendirme
	cout << endl << "Verimli listede tek eleman varsa birkac kere calistirip tekrar deneyebilirsiniz." << endl;

	//dosyayi kapatiyor
	combFile.close();
	//dosyayi siliyor ve silindi mi diye bakiyor
	if (0 == remove("combs.txt")) {
		return 0;
	}
	else {
		cout << "Dosya silinemedi." << endl;
		return -1;
	}
}