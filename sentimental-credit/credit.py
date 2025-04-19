

cardNum = input("Number: ")
firstTwo = int(cardNum[0] + cardNum[1])

len = len(cardNum)
alt = False
sum = 0;

for item in reversed(cardNum):
    num = int(item)
    if alt:
        sum = sum + num
        alt = True
    else:
        num = num * 2
        for n in str(num):
            sum = sum + int(n)
print(sum)

if (sum % 10 != 0):
    print("INVALID\n")


if len == 15 and (firstTwo == 34 or firstTwo == 37):
    print("AMEX\n")
elif len == 16 and firstTwo >= 51 and firstTwo <= 55:
    print("MASTERCARD\n")
elif (len == 13 or len == 16) and firstTwo >= 40 and firstTwo <= 49:
    print("VISA\n")
else:
    print("INVALID\n")


