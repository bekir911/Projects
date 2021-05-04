f = open("teams.txt", "r")
a = list(f.readlines())

for i in range(18):
    a[i] = a[i].strip("\n")

b = 17

for i in range(1, 18):
    print("\n", (2 * i - 1),". Hafta", sep = "")

    for j in range(1, 10):
        print(a[j - 1], a[-j], sep = " - ")

    a.insert(1, a[-1]), a.pop()

    b -= 1

    if b < 1:
        b = 17

    print("\n", 2 * i, ". Hafta", sep = "")

    for j in range(1, 10):
        print(a[-j], a[j-1], sep = " - ")

    a.insert(1, a[-1]), a.pop()

    b -= 1
    
