// VARIABLES ______________________________________

int myNum = 15;

int myNum2; // do not assign, then assign
myNum2 = 15;

int myNum3 = 15; // myNum3 is 15
myNum3 = 10;     // myNum3 is now 10

float myFloat = 5.99; // floating point number
char myLetter = 'D';  // character

int x = 5;
int y = 6;
int sum = x + y; // add variables to sum

int x = 5, y = 6, z = 50; // declare multiple variables

// CONSTANTES ______________________________________

const int minutesPerHour = 60;
const float PI = 3.14;

const int BIRTHYEAR = 1980; // Best Practice

// COMENTARIOS ______________________________________

// this is a comment
printf("Hello World!"); // Can comment anywhere in file

/*Multi-line comment, print Hello World!
to the screen, it's awesome */

// PRINT ____________________________________________

printf("I am learning C.");
int testInteger = 5;
printf("Number = %d", testInteger);

float f = 5.99; // floating point number
printf("Value = %f", f);

short a = 0b1010110; // binary number
int b = 02713; // octal number
long c = 0X1DAB83; // hexadecimal number

// output in octal form
printf("a=%ho, b=%o, c=%lo\n", a, b, c);
// output => a=126, b=2713, c=7325603

// Output in decimal form
printf("a=%hd, b=%d, c=%ld\n", a, b, c);
// output => a=86, b=1483, c=1944451

// output in hexadecimal form (letter lowercase)
printf("a=%hx, b=%x, c=%lx\n", a, b, c);
// output => a=56, b=5cb, c=1dab83

// Output in hexadecimal (capital letters)
printf("a=%hX, b=%X, c=%lX\n", a, b, c);
// output => a=56, b=5CB, c=1DAB83

// CONTROL NUM DE ESPACIOS ______________________

int a1 = 20, a2 = 345, a3 = 700;
int b1 = 56720, b2 = 9999, b3 = 20098;
int c1 = 233, c2 = 205, c3 = 1;
int d1 = 34, d2 = 0, d3 = 23;

printf("%-9d %-9d %-9d\n", a1, a2, a3);
printf("%-9d %-9d %-9d\n", b1, b2, b3);
printf("%-9d %-9d %-9d\n", c1, c2, c3);
printf("%-9d %-9d %-9d\n", d1, d2, d3);

/*
output result:
20        345       700      
56720     9999      20098    
233       205       1        
34        0         23  

In %-9d, d means to output in 10 base, 9 means to occupy at least 9 characters width, and the width is not enough to fill with spaces, - means left alignment
*/
 
// STRINGS ____________________________________

char greetings[] = "Hello World!";
printf("%s", greetings);

//access string
char greetings[] = "Hello World!";
printf("%c", greetings[0]);

//modify string
char greetings[] = "Hello World!";
greetings[0] = 'J';

printf("%s", greetings); // prints "Jello World!"

//Another way to create a string
char greetings[] = {'H','e','l','l','\0'};
printf("%s", greetings); // print "Hell!"

//Creating String using character pointer (String Literals)
char *greetings = "Hello";
printf("%s", greetings); // print "Hello!"

//NOTE: String literals might be stored in read-only section of memory. Modifying a string literal invokes undefined behavior. You can't modify it.!

//C does not have a String type, use char type and create an array of characters

// CONDICIONALES ________________________________

int time = 20;
if (time < 18) {
  printf("Goodbye!");
} else {
  printf("Good evening!");
}
// Output -> "Good evening!"
int time = 22;
if (time < 10) {
  printf("Good morning!");
} else if (time < 20) {
  printf("Goodbye!");
} else {
  printf("Good evening!");
}
// Output -> "Good evening!"

//Ternary operator
int age = 20;
(age > 19) ? printf("Adult") : printf("Teenager");

// SWITCH __________________________________________

int day = 4;

switch (day) {
  case 3: printf("Wednesday"); break;
  case 4: printf("Thursday"); break;
  default:
    printf("Weekend!");
}
// output -> "Thursday" (day 4)

// CICLO WHILE _____________________________________

int i = 0;
while (i < 5) {
  printf("%d\n", i);
  i++;
}

/* NOTE: Don't forget to increment the variable used in the condition, otherwise the loop will never end and become an "infinite loop"!
Do/While Loop */

int i = 0;
do {
  printf("%d\n", i);
  i++;
} while (i < 5);

// CICLO FOR ________________________________________________

for (int i = 0; i < 5; i++) {
  printf("%d\n", i);
}

// USO DE BREAK________________________________________________

//Break out of the loop Break/Continue

for (int i = 0; i < 10; i++) {
  if (i == 4) {
    break;
  }
  printf("%d\n", i);
}

//break out of the loop when i is equal to 4

for (int i = 0; i < 10; i++) {
  if (i == 4) {
    continue;
  }
  printf("%d\n", i);
}
//Example to skip the value of 4

// While Break Example
int i = 0;
while (i < 10) {
  if (i == 4) {
    break;
  }
  printf("%d\n", i);

  i++;
}

// While continue example
int i = 0;
while (i < 10) {
  i++;

  if (i == 4) {
    continue;
  }
  printf("%d\n", i);
}

// ARREGLOS ______________________________________________________

 

int myNumbers[] = {25, 50, 75, 100};

printf("%d", myNumbers[0]);
// output 25

//change array elements
int myNumbers[] = {25, 50, 75, 100};
myNumbers[0] = 33;
printf("%d", myNumbers[0]);

//Loop through the array
int myNumbers[] = {25, 50, 75, 100};
int i;
for (i = 0; i < 4; i++) {
  printf("%d\n", myNumbers[i]);
}

//set array size
int myNumbers[4]; // Declare an array of four integers

// add element
myNumbers[0] = 25;
myNumbers[1] = 50;
myNumbers[2] = 75;
myNumbers[3] = 100;

//Enumeration Enum
Enum week { Mon = 1, Tues, Wed, Thurs, Fri, Sat, Sun };

//define enum variable
enum week a, b, c;
enum week { Mon = 1, Tues, Wed, Thurs, Fri, Sat, Sun } a, b, c;

//With an enumeration variable, you can assign the value in the list to it
enum week { Mon = 1, Tues, Wed, Thurs, Fri, Sat, Sun };
enum week a = Mon, b = Wed, c = Sat;
// or
enum week{ Mon = 1, Tues, Wed, Thurs, Fri, Sat, Sun } a = Mon, b = Wed, c = Sat;

//Enumerate sample applications
enum week {Mon = 1, Tues, Wed, Thurs} day;

scanf("%d", &day);

switch(day) {
  case Mon: puts("Monday"); break;
  case Tues: puts("Tuesday"); break;
  case Wed: puts("Wednesday"); break;
  case Thursday: puts("Thursday"); break;
  default: puts("Error!");
}

// INPUT DE USUARIO________________________________________
// Create an integer variable to store the number we got from the user
int myNum;

// Ask the user to enter a number
printf("Please enter a number: \n");

// Get and save the number entered by the user
scanf("%d", &myNum);

// Output the number entered by the user
printf("The number you entered: %d", myNum);

// ENTRADA STRING DEL USUARIO_____________________________
// create string
char firstName[30];
// Ask the user to enter some text
printf("Enter your name: \n");
// get and save the text
scanf("%s", &firstName);
// output text
printf("Hello %s.", firstName);

// DIRECCION DE MEMORIA_______________________________________________
int myAge = 43;
printf("%p", &myAge);
// Output: 0x7ffe5367e044

//CREAR APUNTADOR____________________________________________
int myAge = 43; // an int variable
printf("%d", myAge); // output the value of myAge(43)

// Output the memory address of myAge (0x7ffe5367e044)
printf("%p", &myAge);

// VARIABLE APUNTADOR______________________________________________
int myAge = 43; // an int variable
int*ptr = &myAge; // pointer variable named ptr, used to store the address of myAge

printf("%d\n", myAge); // print the value of myAge (43)

printf("%p\n", \&myAge); // output the memory address of myAge (0x7ffe5367e044)
printf("%p\n", ptr); // use the pointer (0x7ffe5367e044) to output the memory address of myAge

// OPERADORES ARITMETICOS_________________________________

int myNum = 100 + 50;
int sum1 = 100 + 50; // 150 (100 + 50)
int sum2 = sum1 + 250; // 400 (150 + 250)
int sum3 = sum2 + sum2; // 800 (400 + 400)

/*
+ 	Add 	x + y
- 	Subtract 	x - y
* 	Multiply 	x * y
/ 	Divide 	x / y
% 	Modulo 	x % y
++ 	Increment 	++x
-- 	Decrement 	--x

*/

/*
OPERADORES DE COMPARACION
== 	equals 	x == y
!= 	not equal to 	x != y
> 	greater than 	x > y
< 	less than 	x < y
>= 	greater than or equal to 	x >= y
<= 	less than or equal to 	x <= y
*/

//TIPOS DE DATOS______________________________
char 	character type
short 	short integer
int 	integer type
long 	long integer
float 	single-precision floating-point type
double 	double-precision floating-point type
void 	no type

// FORMATOS________________________________________________
%d or %i 	int integer
%f 	float single-precision decimal type
%lf 	double high precision floating point data or number
%c 	char character
%s 	for strings strings
Basic format specifiers
Octal 	%ho 	%o 	%lo
Decimal 	%hd 	%d 	%ld
Hexadecimal 	%hx /%hX 	%x /%X 	%lx /%lX
Data format example

// EJEMPLOS:

int myNum = 5;
float myFloatNum = 5.99; // floating point number
char myLetter = 'D';     // string
// print output variables
printf("%d\n", myNum);
printf("%f\n", myFloatNum);
printf("%c\n", myLetter);

// FUNCIONES________________________________________________________
 

int main(void) {
  printf("Hello World!");

  return 0;
}

// La funcion esta compuesta por dos partes

void myFunction() { // declaration declaration
  // function body (code to be executed) (definition)
}

/*
Declaration declares the function name, return type and parameters (if any)
Definition function body (code to execute)
*/
 

// function declaration
void myFunction();
// main method
int main() {
  myFunction(); // --> call the function

  return 0;
}

void myFunction() {// Function definition
  printf("Good evening!");
}

//Call function

// create function
void myFunction() {
  printf("Good evening!");
}

int main() {
  myFunction(); // call the function
  myFunction(); // can be called multiple times

  return 0;
}
// Output -> "Good evening!"
// Output -> "Good evening!"

// PARAMETROS DE FUNCION__________________________________________
void myFunction(char name[]) {
  printf("Hello %s\n", name);
}

int main() {
  myFunction("Liam");
  myFunction("Jenny");

  return 0;
}
// Hello Liam
// Hello Jenny

//Multiple parameters

void myFunction(char name[], int age) {
  printf("Hi %s, you are %d years old.\n",name,age);
}
int main() {
  myFunction("Liam", 3);
  myFunction("Jenny", 14);

  return 0;
}
// Hi Liam you are 3 years old.
// Hi Jenny you are 14 years old.

//Return value
int myFunction(int x) {
  return 5 + x;
}

int main() {
  printf("Result: %d", myFunction(3));
  return 0;
}
// output 8 (5 + 3)

//two parameters
int myFunction(int x, int y) {
  return x + y;
}

int main() {
  printf("Result: %d", myFunction(5, 3));
  // store the result in a variable
  int result = myFunction(5, 3);
  printf("Result = %d", result);

  return 0;
}
// result: 8 (5 + 3)
// result = 8 (5 + 3)

//EJEMPLO RECURSIVIDAD_____________________________________________
int sum(int k);

int main() {
  int result = sum(10);
  printf("%d", result);

  return 0;
}

int sum(int k) {
  if (k > 0) {
    return k + sum(k -1);
  } else {
    return 0;
  }
}


// FUNCIONES MATEMATICAS ______________________________ 

#include <math.h>

void main(void) {
  printf("%f", sqrt(16)); // square root
  printf("%f", ceil(1.4)); // round up (round)
  printf("%f", floor(1.4)); // round up (round)
  printf("%f", pow(4, 3)); // x(4) to the power of y(3)
}