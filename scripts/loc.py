class LOC:
    files: int = 0
    blank: int = 0
    rem:   int = 0
    code:  int = 0

    def __init__(self, name: str):
        print()
        self.files = int(input(f"Enter {name} file count: "))
        self.blank = int(input(f"Enter {name} blank lines: "))
        self.rem   = int(input(f"Enter {name} comment lines: "))
        self.code  = int(input(f"Enter {name} code lines: "))

    def add_files(self, other):
        return self.files + other.files

    def add_lines(self, other):
        return self.blank + self.rem + self.code + other.blank + other.rem + other.code

    def display_total(self, other):
        print()
        print(f"Total files: {self.add_files(other)}.")
        print(f"Total code:  {self.add_lines(other)}.")

def main():
    cpp     = LOC("main")
    shaders = LOC("shader")

    cpp.display_total(shaders)

if __name__ == '__main__':
    main()