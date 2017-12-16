import cs50


def get_height():
    while True:
        print("Height: ")
        i = cs50.get_int()
        if i >= 0 and i <= 23:
            return i
        
def main():
    make_pyramid(get_height())
    
def make_pyramid(height):
    bl = 1
    for i in range(height):
        sp = height - bl
        print(' '*sp+'#'*bl+'  '+'#'*bl)
        bl += 1
        
if __name__ == '__main__':
    main()