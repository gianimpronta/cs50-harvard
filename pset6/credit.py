import cs50

def main():
    print('Number: ')
    number = cs50.get_int()
    if (first_check(number) % 10) != 0:
        print("first INVALID")
        return
    sec_check(number)
    return
    
def first_check(ccn):
    sum = 0
    sum += sec_to_last(ccn)
    sum += remaining(ccn)
    return sum

def remaining(ccn):
    sum = 0
    for i in range(len(str(ccn))):
        x = ccn % 10
        sum += x
        ccn //= 100
    return sum

def sec_check(ccn):
    if len(str(ccn)) == 13:
        print("VISA")
    elif len(str(ccn)) == 15:
        ccn //= 10000000000000
        if ccn == 34 or ccn == 37:
            print("AMEX")
            return
    elif len(str(ccn)) == 16:
        ccn //= 100000000000000
        if ccn in [51, 52, 53, 54, 55]:
            print('MASTERCARD')
            return
        elif ccn // 10 == 4:
            print("VISA")
            return
    else:
        print("INVALID")    
    
def sec_to_last(ccn):
    sum = 0
    for i in range(len(str(ccn))):
        x = (ccn // 10) % 10
        x *= 2
        if (x >= 10):
            x = ((x // 10) + (x % 10)) 
        sum += x
        ccn //= 100
    return sum
    
if __name__ == '__main__':
    main()