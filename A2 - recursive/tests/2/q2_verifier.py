import sys

inputFilePath = sys.argv[1]
outputFilePath = sys.argv[2]

def getInput():
    with open(inputFilePath, "r") as inputFile:
        lines = inputFile.readlines() 
    nums = [int(x) for x in lines[1].split()]
    return nums

def parse_expression(expression):
    numbers = []
    operators = []
    current_number = ""

    def is_operator(char):
        return char in "+-*="

    for char in expression:
        if char.isspace():
            continue
        if char.isdigit():
            current_number += char
        elif is_operator(char):
            if current_number:
                numbers.append(int(current_number))
                current_number = ""
            operators.append(char)
        else:
            sys.exit(-1)
            pass

    if current_number:
        numbers.append(int(current_number))

    return numbers, operators

def getOutput():
    with open(outputFilePath, "r") as outputFile:
        lines = outputFile.readlines() 
    return parse_expression(lines[0])

def checkEqualityOfNums(list1, list2):
    if len(list1) != len(list2):
        sys.exit(-1)

    for i in range(len(list1)):
        if list1[i] != list2[i]:
            sys.exit(-1)

def checkValidityOfOperators(operators, nums):
    if len(operators) != (len(nums)-1):
        sys.exit(-1)
    if operators[-1] != '=':
        sys.exit(-1)

def checkEqualityOfEquation(operators, nums):
    curr = nums[0]
    for i in range(0,len(operators)-1):
        if(operators[i]=='+'):
            curr = curr + nums[i+1]
        elif(operators[i]=='-'):
            curr = curr - nums[i+1]
        elif(operators[i]=='*'):
            curr = curr * nums[i+1]
    if(curr != nums[-1]):
        sys.exit(-1)    

def main():
    inputNums = getInput()
    outputNums, outputOperators = getOutput()    
    checkEqualityOfNums(inputNums, outputNums)
    checkValidityOfOperators(outputOperators, inputNums)
    checkEqualityOfEquation(outputOperators, outputNums)
    sys.exit(0)        

if __name__ == "__main__":
    main()