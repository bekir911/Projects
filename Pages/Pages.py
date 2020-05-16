import random
from itertools import permutations

#burayi biliyorsun zaten
def makeList(numberOfStudents = 5, numberOfPages = 250):
    list1 = []

    for i in range(numberOfStudents):
        #son elemana kadar olmasin diye bir eksigi var
        list2 = [random.randint(1, numberOfPages - 1)]
        #o eksik elemandan sonuna kadar gidiyo iste
        list2.append(random.randint(list2[0] + 1, numberOfPages))
        list1.append(list2)

    return list1

#arkadas yapmisti ben de yapam dedim
if __name__ == "__main__":
    #input olarak aldim, yorumlu satirlari da kullanabilirsin
    numberOfStudents = int(input("Enter number of students: "))
    numberOfPages = int(input("Enter number of pages: "))
    #numberOfStudents = 5
    #numberOfPages = 250

    #fonkisyonu cagiriyo
    pageNumber = makeList(numberOfStudents, numberOfPages)
    #kac sayfa okuduklarini tutacak liste
    pageLen = []

    #kac sayfa okuduklarini hesapliyor
    for i in range(len(pageNumber)):
        pageLen.append(pageNumber[i][1] - pageNumber[i][0])

    #tum ihtimalleri tutacak liste
    #icinde listeler olacak, bu listeler her bir senaryoyu icerecek
    results = []
    
    #permutasyonlari tutan liste
    #icinde tuple var
    perm = list(permutations(range(numberOfStudents)))

    #permutasyondan gelenlerin ilkini atadim
    #cunku elimde kiyas durumu var
    #iceridekiyle yeni gelecek olani kiyaslayabiliyorum
    #icerisi bos olursa kiyaslayamam
    for i in range(len(perm)):
        tempList = [perm[i][0]]
        results.append(tempList)  

    #iki ogrencinin sayfalari kesisiyorsa bu degiskeni artiracagim
    resultCounter = 0
    #permutasyondaki listeleri gezmek icin i
    for i in range(len(perm)):
        #secilen permutasyondaki elemanlari gezmek icin j
        #1 den basliyor cunku ilk elemanini yukarida koyduk
        for j in range(1, numberOfStudents):
            #resultsdaki elemanlari gezmek icin
            #yeni eklenecek eleman resultsdaki her bir eleman ile
            #ayri ayri bakildiginda kesismemeli
            for k in range(len(results[i])):
                #yuksek degerlerini karsilastiriyorum
                #ona gore yuksekte olan max oluyor
                if pageNumber[results[i][k]][1] > pageNumber[perm[i][j]][1]:
                    maxValue = pageNumber[results[i][k]][1]
                else:
                    maxValue = pageNumber[perm[i][j]][1]

                #alcak degerlerine gore de min oluyor
                if pageNumber[results[i][k]][0] < pageNumber[perm[i][j]][0]:
                    minValue = pageNumber[results[i][k]][0]
                else:
                    minValue = pageNumber[perm[i][j]][0] 
                
                #uzunluklari toplami aralarindaki mesafeyi geciyorsa cakisiyorlardir
                #bu yuzden degiskeni artiriyorum
                if (pageLen[results[i][k]] + pageLen[perm[i][j]]) > (maxValue - minValue):
                    resultCounter += 1

            #results'un i ninci senaryosundaki tum elemanlar ile kesismiyorsa ucuna ekliyorum
            if resultCounter == 0:
                results[i].append(perm[i][j])
            #diger kiyaslama icin sifirliyorum
            resultCounter = 0

    #bu asamada results icinde tum ihtimaller var 
    #bu ihtimaller cakismayanlari iceriyor

    #result un i ninci senaryosundaki toplam degeri tutacak liste
    totals = []
    #senaryo basina toplam degeri tutacak degisken
    total = 0
    #results un uzunlugu belli zaten
    for i in range(len(results)):
        #ama i ninci senaryonun uzunlugu belli degil o yuzden boyle
        #gerisinde bisi yok
        for j in range(len(results[i])):
            total += pageLen[results[i][j]]
        totals.append(total)
        total = 0

    #bubble sort ile siralama yapiyor
    #eksi bir falan o yuzden var
    for i in range(len(results) - 1):
        for j in range(len(results) - i - 1):
            if totals[j] < totals[j + 1]:
                #internette pythonda liste swap nasil yapilir dedim bunu dedi
                #her ne kadar 2 degisken icin bilsem de
                #boyle de kullanilabilecegini dusunmemistim
                totals[j], totals[j + 1] = totals[j + 1], totals[j]
                results[j], results[j + 1] = results[j + 1], results[j]
    
    #burda da yazdiriyo iste
    #sifirincilar cunku siralandi
    print("Number of longest non-overlaping pages: ", totals[0])
    #ogrenciler direkt print(results[0])
    #seklinde de yazdirilabilir ama guzel durmaz diye boyle yaptim
    print("Students: ", end = '')
    for i in results[0]:
        print(i + 1, end = ' ')
    #bunlar hesaplandiklari an da basitirlabilirler
    print("\nPage Number: ", pageNumber)
    print("page Length: ", pageLen)
    

