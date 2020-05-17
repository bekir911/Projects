#include <iostream>
#include <chrono>

using namespace std;

//yazdirmasini istemiyorsan yorumla
//#define PRINT

//asal mi diye kontrol eden fonkisyon
//cift sayilar olmadigi icin basta 2 yi kontrol ediyor
bool isPrime(long long number) {
	if (0 == number % 2)
		return false;

	//5ten basliyor karesine kadar olan potansiyel asallara boluyor
	//potansiyel asallar 6n-1 ve 6n+1
	//bu yuzden dongude 4 eklerken iceride 2 ekliyor
	for (long long i{ 3 }; i * i <= number; i += 2) {
		if (0 == number % i)
			return false;
	}

	return true;
}

int main() {

	//array size belirtiyor. Bu kadar sayi tutabilir icinde
	//1 milyardan kücük asal sayilar daha az oldugu icin sorun yok
	//orijinalde 50'847'534 olmali
	//constexpr auto ARRAY_SIZE = 50'850'000;
	//int array'i olusturuyor, deger 1 milyari tutabildigi icin sorun yok
	//int* array = new int[ARRAY_SIZE];
	//bolme islemini dizideki elemanlardan yaptigi icin ilk degere ihtiyac duyuyor
	//array[0] = 2;

	//max, kullanicinin girdigi sayi, o sayiya kadar hesap yapilir
	auto max{ 0 };

	//kullanicidan sayi alip degiskene atiyor
	cout << "Enter number: ";
	cin >> max;
	
	//pi(x)'ten biraz buyuk bir deger elde ediliyor
	//fazladan yer almak gereksiz ve zaman harciyor
	max = max / (log(max) - 2);
	
	//dizi olusturuyor
	double* array = new double[max];

	//dizinin ilk degerini atiyor
	//sonraki islemler dizinin elemanlari ile yapilacagi icin ilk eleman gerek
	array[0] = 2;

	//icinde kac sayi oldugunu tutmasi icin degisken
	//ustte ilk degeri tanimlandigi icin 1
	auto arraySize{ 1 };
	//flagboluyorsa artirip tespit etmek icin
	auto flag{ 0 };


	//sayac baslatiyor, ilk satir cikti uclu olarak bolunsun diye
	cout.imbue(locale(""));
	auto start = std::chrono::steady_clock::now();

	//ilk degerden sonraki 3 oldugu icin ilk degeri 3
	//cift sayilar asal olamadigi icin 2 ekliyor
	for (auto i{ 3 }; i <= max; i += 2) {

		//bolen sayi karekokunden buyuk ise anlami yok
		for (auto j{ 0 }; array[j] * array[j] <= i; ++j) {

			//asal boleni varsa degiskeni artirip cikiyor
			if (0 == i % static_cast<int>(array[j])) {
				++flag;
				break;
			}
		}

		//degisken artirilmissa diziye ekliyor ve array boyutunu artiriyor
		if (0 == flag)
			array[arraySize++] = i;

		//degisken artmis olabilir bu yuzden sifirliyor
		flag = 0;
	}

	//toplam degeri tutacak
	long long total{ 0 };

	//toplam degeri tum asallarin toplami yapiyor
	for (auto i{ 0 }; i < arraySize; ++i)
		total += array[i];

	//toplam asal mi diye bakiyor
	//degilse en buyuk asali cikartip tekrar bakiyor
	for (auto i{ arraySize - 1 }; i > 0; --i) {

		//toplam cift ise atliyor, asal mi diye bakmiyor
		if (0 == i % 2) {
			total -= array[i];
			continue;
		}

		//toplam asal mi diye bakiyor
		if (isPrime(total)) {
			cout << "Sum value: " << total << endl;
			cout << "Number: " << array[i + 1] << endl;
			cout << "If the Number is something broken, the prime number that comes after this is the answer: " << array[i] << endl;
			break;
		}

		//son elemani toplamdan cikartiyor
		total -= array[i];
	}

	//sayaci durduruyor ve hesaplanan sureyi duration degiskenine atiyor
	auto stop = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

	//sureyi bastiriyor
	cout << "Duration: " << duration << " us" << endl;

	//array boyutunu bastiriyor
	//array boyutu simdilik onemli degil
	//istege bagli olarak program bir sayidan
	//daha ufak asal sayisini bulman icin kullanilabilir
	//o zaman bu deger ise yarar
	cout << "Number of primes: " << arraySize << endl;

	//ekrana yazdiriyor yukaridan acilabilir tanimi
#ifdef PRINT
	cout << endl;

	for (auto i{ 0 }; i < arraySize; ++i)
		cout << "index: " << i + 1 << "\tValue: " << array[i] << endl;

#endif PRINT
	
	//bellekten alinan yeri birakiyor
	delete[] array;
	
	return 0;
}
