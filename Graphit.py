import math 
import matplotlib.pyplot as plt
import numpy as np
from fractions import Fraction
#imported to allow graphs, functions, and math to work

#function that allows basic math to work
def calculate(x,o,y):
    if o == 1:
        return x + y
    elif o == 2:
        return x-y
    elif o == 3:
        return x*y
    elif o == 4:
        if y == 0:
            err = 'ERROR You can not divide by zero'
            return err
        else:
            return x/y

#function that allows for user to do basic math with fractions
def fractions(f1,o,f2):
    if o == 1:
        a = f1 + f2
        return a
    elif o == 2:
        b = Fraction(f1) - Fraction(f2)
        return b
    elif o == 3:
        c = Fraction(f1) * Fraction(f2)
        return c
    elif o == 4:
        if f2 == 0:
            err = 'You can not divide by zero'
            return err
        else:
            d = Fraction(f1) / Fraction(f2)
            return d

on = None
history = []
print('Welcome to Graphit! ')
operand_num = [1,2,3,4,5,6]

#while loop that allows the user to use the calculator for as long as they would like
while on == None:
    print('"Choose one of the following operations:')
    print('1 - Add')
    print('2 - Subtract')
    print('3 - Multiply')
    print('4 - Divide')
    print('5 - Fractions')
    print('6 - Graph')

    user = int(input('OPERATION> '))

    if user not in operand_num:
        print('Please enter a valid operation. ')
    elif user != 5 and user != 6:
            first_num = float(input('What is the first number? '))
            second_num = float(input('What is the second number? '))
            print(calculate(first_num,user,second_num))
            history.append(calculate(first_num,user,second_num))
# This is a lengthy bit of code used for the graphing section, yes some of this problem should have been put into a function
    if user == 6:
        # This gets an input from the user, puts it in the history tape, and asks if they would like to add another function to graph
        fxn = input('Input a function> ')
        history.append(fxn)
        af = input('Would you like to input another function? Yes or No? ')
        af.lower()
        # This is code for the second function and is only run if the user says yes to the input above
        if af == 'yes':
            gxn = input('Input a function> ')
            history.append(gxn)
            gxn = list(gxn)
            # This allows the user to use ^ instead of having to know that the power in python is **
            for i in range(len(gxn)):
                if gxn[i] == '^':
                    gxn.remove(gxn[i])
                    gxn.insert(i,'**')
                #  This allows the user to use e in their functions
                if gxn[i] == 'e':
                    gxn.remove(gxn[i])
                    gxn.insert(i,'math.e')
            # This bit of code here allows the user to input 4x and have it read as 4*x so that it can be graphed
            for i in range(len(gxn)-1):
                if gxn[i].isdigit():
                    if gxn[i+1].isalpha():
                        gxn.insert(i+1,'*')
            # This allows the user to be able to input somethng such as 4e and the calculator to read it as 4*math.e
            for i in range(len(gxn)-1):
                if gxn[i].isdigit():
                    if gxn[i+1] == 'math.e':
                        gxn.insert(i+1,'*')
            gxn = ''.join(gxn)
        # This code does the same as the gxn function, but will still run even if the user doesn't say yes to another function
        fxn = list(fxn)
        for i in range(len(fxn)):
            if fxn[i] == '^':
                fxn.remove(fxn[i])
                fxn.insert(i,'**')
            if fxn[i] == 'e':
                fxn.remove(fxn[i])
                fxn.insert(i,'math.e')
        for i in range(len(fxn)-1):
            if fxn[i].isdigit():
                if fxn[i+1].isalpha():
                    fxn.insert(i+1,'*')
        for i in range(len(fxn)-1):
                if fxn[i].isdigit():
                    if fxn[i+1] == 'math.e':
                        fxn.insert(i+1,'*')
        fxn = ''.join(fxn)
        # This allows the user to input bounds 
        neg = int(input('Insert the min bound> '))
        pos = int(input('Insert the max bound> '))
        
        # This code styles the graph and makes it look nice and pretty
        if af == 'yes':
            x = np.arange(neg,pos, 1)
            f = eval(fxn)
            g = eval(gxn)
            fig, ax = plt.subplots()
            ax.plot(x, f, linewidth=2.0, c='blue',label = fxn )
            ax.plot(x,g, linewidth=2.0, c='red', label = gxn)

            plt.xlabel('X-axis')
            plt.ylabel('Y-axis')

            plt.title('Graphit ')
            plt.axvline(0, c='black', ls='--')
            plt.axhline(0, c='black', ls='--')
            plt.legend()
            plt.show()
        else:
            x = np.arange(neg,pos, 1)
            f = eval(fxn)
            fig, ax = plt.subplots()
            ax.plot(x, f, linewidth=2.0, c='blue',label = fxn )

            plt.xlabel('X-axis')
            plt.ylabel('Y-axis')

            plt.title('Graphit ')
            plt.axvline(0, c='black', ls='--')
            plt.axhline(0, c='black', ls='--')
            plt.legend()
            plt.show()
# Fraction code that allows for the user to easily input fractions and choose what they would like to do with it. 
    if user == 5:
        f_1 = input('First Fraction> ')
        f_2 = input('Second Fraction> ')
        final_1 = Fraction(f_1)
        final_2 = Fraction(f_2)
    # Here a new operation is asked because I needed to know if the user wanted to do just fractions or just floats/decimals before
        print('"Choose one of the following operations:')
        print('1 - Add')
        print('2 - Subtract')
        print('3 - Multiply')
        print('4 - Divide')
        operator = int(input('OPERATION> '))
        print(fractions(final_1,operator,final_2))
        u = (fractions(final_1,operator,final_2))

        history.append(str(u))
# Checks if the user is done using the calculator or if they would like to continue
    ctrl = input('CONTINUE? Yes or No? ')
    ctrl = ctrl.lower()
    if ctrl == 'yes':
        continue
# Output of the user's previous calculations
    else:
        print('Here is your calculation history tape: ')
        print(f'HISTORY: {history}')
        print('Goodbye! ')
        break