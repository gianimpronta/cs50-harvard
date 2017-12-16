import sys
import crypt


def main(arg):
    if len(arg) < 2:
        print("Missing command-line argument")
        return 1
    elif len(arg) > 2:
        print("Too much command-line arguments")
        return 1
    hash = arg[1]    
    chars = "./0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    salt = ['','']
    pswd = ['','','','']
    salt[0] = hash[0]
    salt[1] = hash[1]
    salt = ''.join(salt)
    for c1 in range(12, 64):
        for c2 in range(12, 64):
            for c3 in range(12, 64):
                for c4 in range(12, 64):
                    for i in range(1,4):
                        if i == 0:
                            pswd[0] = chars[c1]
                        elif i == 1:
                            pswd[0] = chars[c1]
                            pswd[1] = chars[c2]
                        elif i == 2:
                            pswd[0] = chars[c1]
                            pswd[1] = chars[c2]
                            pswd[2] = chars[c3]
                        elif i == 3:
                            pswd[0] = chars[c1]
                            pswd[1] = chars[c2]
                            pswd[2] = chars[c3]
                            pswd[3] = chars[c4]
                        pswd = ''.join(pswd)
                        result = crypt.crypt(pswd, salt)
                        if hash == result:
                            print(pswd)
                            return
                        pswd = ['','','','']
    print('Not Cracked')
    
if __name__ == '__main__':
    main(sys.argv)