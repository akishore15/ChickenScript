import subprocess

def run_lexer(code):
    with open('sharkara_code.txt', 'w') as file:
        file.write(code)
    lexer_output = subprocess.run(['./lexer', 'sharkara_code.txt'], capture_output=True, text=True)
    return lexer_output.stdout

def run_parser(tokens):
    with open('sharkara_tokens.txt', 'w') as file:
        file.write(tokens)
    parser_output = subprocess.run(['./parser', 'sharkara_tokens.txt'], capture_output=True, text=True)
    return parser_output.stdout

def main():
    print("Welcome to the Sharkara Terminal")
    while True:
        code = input("Sharkara> ")
        if code.lower() in ['exit', 'quit']:
            print("Exiting Sharkara Terminal.")
            break
        lexer_output = run_lexer(code)
        print("Lexer Output:\n", lexer_output)
        
        parser_output = run_parser(lexer_output)
        print("Parser Output:\n", parser_output)

if __name__ == "__main__":
    main()
