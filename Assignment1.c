/**
 * Summary: This program finds unbooked exam slots from an array. It is able to book new exam slots and check if there is free space for that booking. 
 *          It can also display exam bookings for certain students by their id.
 * Student Name: Aleksander Naumowicz
 * Student Number: 041039693
 * Course: CST8234 - C Language
 * CET-CS-Level 3
 * Declaration: I declare that this is my own original work and is free from plagarism
 * @author Aleksander Naumowicz
 * 
*/
#include "041039693.h"
#include <stdio.h>
#include <stdlib.h>

#define ARRAYSIZE (sizeof (examBookings) / sizeof (examBookings[0]))
#define HOURS 8
#define WEEK1 0
#define WEEK2 1


typedef unsigned long int uLLInt;

void displayStudentExamSchedule(int[][5], int);
void formatStudentExamSchedule(int, int[][5]);
void displayMenu();
void displayUnbookedTimes();
void convertArrayToULLInt();
void setExamBookingTime();
uLLInt bitwiseShift(int, int, int);
void uLLIntToArray(uLLInt, int);
void insertExamBooking(int, int, int, int, int, int);
uLLInt binaryReservations[ARRAYSIZE][2] = {0};
uLLInt week1Master = 0;
uLLInt week2Master = 0;

/*
Entry point of the program. Main holds the menu control and display of the program.
*/
int main(void){

int menuControl = 1;
int menuChoice = 0;


while (menuControl){


displayMenu();

scanf("%d", &menuChoice);


switch (menuChoice){

case 1:
    displayStudentExamSchedule(examBookings, 0);
    break;

case 2:
    setExamBookingTime();
    break;

case 3:
    printf("\nFree exam bookings: \n");
    displayUnbookedTimes();
    break;

case 4:
    menuControl = 0;
    break;

default:

    printf("Invalid menu option. Try again.\n");
    break;

}

}


}

/*
This function prints a formatted menu.
*/
void displayMenu(){
printf("\nSelect a menu option:\n1. Display a student's exam schedule\n2. Load a new exam booking\n3. Find unbooked times\n4. exit\n");
}


/*
This function is passed an array and a menu option.
If menu option 0 is passed thru, it asks the user for a student id and then loops thru the entire exam bookings array. It prints every exam booking with a matching student it.
If menu option 1 is passed thru, it prints the unbooked exam booking that is passed thru by an array.
*/
void displayStudentExamSchedule(int array[][5], int menuOption){

// if displaying a student's schedule, proceed
if (menuOption == 0){
    int studentId = 0;

    // taking input for student it
    printf("Enter student id: ");
    scanf("%d", &studentId);

    // printing student number and looping thru entire exam bookings array to find exam times.
    printf("\nExam bookings for student number %d:\n", studentId);
    for (int i = 0; i < ARRAYSIZE; i++){

        if (examBookings[i][4] == 0){
            break;
        }

        if (examBookings[i][0] == studentId){

            formatStudentExamSchedule(i, examBookings);
        }
    }

// if displaying unbooked times, proceed
} else {
    // printing passed thru array (unbooked times array)
    formatStudentExamSchedule(0, array);
}

}


/*
This function takes in an indexing value and an array. 
Then uses both variables to print the exam schedule of a student or print unbooked times.
*/
void formatStudentExamSchedule(int index, int array[][5]){

int week = array[index][1];
int day = array[index][2];
int start = array[index][3];
int hours = array[index][4];

// switch case for week output
switch(week){
    case 0:
        printf("Week 1:  ");
        break;
    case 1:
        printf("Week 2:  ");
        break;
}

// switch case for day output
switch(day){
    case 0:
        printf("Monday ");
        break;
    case 1:
        printf("Tuesday ");
        break;
    case 2:
        printf("Wednesday ");
        break;
    case 3:
        printf("Thursday ");
        break;
    case 4:
        printf("Friday ");
        break;
    case 5:
        printf("Saturday ");
        break;
    case 6:
        printf("Sunday ");
        break;
}
printf("%d:00 - %d:00\n", start, start + hours);
}


/*
This function loops thru the exambookings array looking for each student by their id. When their id is found, it's bitshifted into a bitmask and XOR'd to a holder variable.
It then inserts the final week 1 & 2 bitmask for that student into a binaryReservations array.
Then it OR's every week 1 & 2 bitmask of each student into a global "master" variable that holds the final bitmask of each unbooked slot.
*/
void convertArrayToULLInt(){

// storage var
uLLInt block;
uLLInt blockHolder1 = 0;
uLLInt blockHolder2 = 0;

// looping thru every student number
for (int i = 0; i < 675; i++){
    // looping thru every booking in the array
    for (int j = 0; j < ARRAYSIZE; j++){
        // if booking student id in the array matches the current student id being searched for, proceed.
        if (examBookings[j][0] == i){
            // If exam booking is for week 1, proceed
            if (examBookings[j][1] == 0){
                // setting the bitmask of the booking into a holder var
                blockHolder1 = blockHolder1 ^ bitwiseShift(examBookings[j][4], examBookings[j][2], examBookings[j][3]);
            // if exam booking is for week 2, proceed    
            } else {    
                // setting the bitmask of the booking into a holder var
                blockHolder2 = blockHolder2 ^ bitwiseShift(examBookings[j][4], examBookings[j][2], examBookings[j][3]);
            }
        }
    }

    // inserting the final bitmask of week 1 & 2 for each student into the binary array.
    binaryReservations[i][WEEK1] = blockHolder1;
    binaryReservations[i][WEEK2] = blockHolder2;
}

// looping thru every student in the binary holder array to update week 1 & 2 master variables
for (int i = 0; i < 675; i++){
    // if value isn't empty, proceed
    if (binaryReservations[0] != 0){
        // ORing every binary week with the master
        week1Master = binaryReservations[i][0] | week1Master;
        week2Master = binaryReservations[i][1] | week2Master;
    } else {
        break;
    }
}
}


/*
This function creates a bitmask for an exam slot by using passed thru exam slot values.
It shifts the bitmask right by the amount of hours, inverts it, then shifts it right by the day and starting time.
*/
uLLInt bitwiseShift (int hours, int day, int startTime){

uLLInt block;

// This block of code creates a bitwise mask of 1111111... etc
// Then it shifts to the right by the amount of hours in the block.
// Then it invers or NOT's, flipping the 1's to 0's and 0's to 1's.  0001 1111 =>  1110 0000
// Then it shifts the bits to the right byte wise. So if the booking is on wednesday: 8bits * 2 = 2 bytes over. OR 1110000 00000000 00000000 => 00000000 00000000 11100000000
// Then it shifts the bits to right by the starting time of the exam to get the proper booked bits / exam slot.
block = 0xFFFFFFFFFFFFFFFF;
block >>= (hours);
block = ~block;
block >>= (HOURS * day);
block >>= (startTime - 9);

return block;



}

/*
This function displays the unbooked times by first calling convertArrayToULLInt in order to update the binary array and week 1 & 2 master variables.
Then it passes week 1 & 2 master variables to uLLIntToArray in order to be converted into a parsable array format and printed. 
*/
void displayUnbookedTimes (){
// updating the binary array and master week variables.
convertArrayToULLInt();

// converting master week variables into array format
uLLIntToArray(week1Master, 0);
uLLIntToArray(week2Master, 1);
}

/*
This function takes in user input for a new exam booking and then converts the new exam booking values into a bit mask.
It then ANDs the new bitmask against the week 1 or 2 masters to see if there's any conflicts.
If no conflicts occur, booking is created. If not, error message is displayed.
*/
void setExamBookingTime(){

uLLInt block = 0;


int index = 0;

int studentId = 0;
int examWeek = 0;
int examInput = 0;
int examDay = 0;
int examStartTime = 0;
int examHours = 0;


// finding index of first empty spot in array
for (int i = ARRAYSIZE - 1; i >= 0; i--){
    if (examBookings[i][4] != 0){
        index = i + 1;
        break;
    }
}

// getting inputs
printf("\nEnter student number: ");
scanf("%d", &studentId);

printf("\nEnter exam week (1 or 2): ");
scanf("%d", &examInput);

// changing week to array inputs
examWeek = (examInput > 1) ? 1 : 0;

printf("\nEnter exam day(M = 0; T = 1...): ");
scanf("%d", &examDay);

printf("\nEnter exam start time (9 - 17): ");
scanf("%d", &examStartTime);

printf("\nEnter exam length: ");
scanf("%d", &examHours);

// updating the master weeks and binary arrays.
convertArrayToULLInt();

// creating an exam block to be tested against the master weeks.
block = bitwiseShift(examHours, examDay, examStartTime);


if (examWeek == 0){

    // ANDing the new exam booking against the master. If it's for a valid time no 1's should be copied.
     ((week1Master & block) != 0) ? printf("\nThat timeblock is not available.") : insertExamBooking(index, studentId, examWeek, examDay, examStartTime, examHours);

} else {

    ((week2Master & block) != 0) ? printf("\nThat timeblock is not available.") : insertExamBooking(index, studentId, examWeek, examDay, examStartTime, examHours);
    
}
}


/*
This function accepts a week bitmask (uLLInt) and a week specifier (int).
The function then reverses the bits of the passed thru bitmask in order to have the schedule ordered from right to left in binary.
The function keeps track of bytes (days) using a counter 
The function then iterates over each bit in the mask in a loop, and checks for a 1 or 0 by % 2. It keeps track of unbooked bits by using a counter. It iterates thru the unbooked bits in a while loop until a booked bit is found.
It keeps track of unbooked bits and starting time using a counter. 
It then inserts the unbooked slot values into an array, and calls displayStudentExamSchedule to print it.
*/
void uLLIntToArray(uLLInt week, int weekNum){

// Declaring an array to store unbooked time values.
int unbookedArray[ARRAYSIZE][5] = {0};

// Declaring counter variables. byteCounter is set to -1 because I increment it at the beginning of the while loop. So it's value is actually 0.
int bitCounter = 0;
int placeCounter = 0;
int byteCounter = -1;

// onBooking is used as a boolean to check if I am on an empty booking or not.
int onBooking = 0;

// temp holds the long decimal number to be divided over.
uLLInt temp = 0;


// This loop reverses the bits so that monday is at the "beginning" of the binary. 
// week >> i   bit shifts week i spots to the right, this is done to get first to last bit in the decimal string. 
// Then you & 1 the bit so booked slot is copied.
// << (64 - i - 1) shifts the bit by the difference to the left.
// So if i = 0 the first bit gets shifted 63 spots to the left, which is it's proper position at the end of the reversed binary number.
// The reversed number result in binary is:
//      Monday vvv                                             Sunday vvv
// Original: 00111111 11100011 00111111 11100000 11100000 00000111 00000000 00000000
// Reversed: 00000000 00000000 11100000 00000111 00000111 11111100 11000111 11111100
//               Sunday ^^^                                            Monday ^^^

/*
Code provided by CodeVault.
YouTube. (2019). Reversing all the bits in an integer. YouTube. Retrieved July 5, 2022, from https://www.youtube.com/watch?v=i4mB2tjeRes.
*/
for (int i = 0; i < 64; i++){
temp |= ((week >> i) & 1) << (64 - i - 1);
}


// This loops until the value in temp is 0. Temp is divided by 2 to drop bits.
while (temp > 0){

    // At the beginning of each byte/day, increment the day by 1 and reset counters.
    byteCounter++;
    placeCounter = 0;
    bitCounter = 0;

    // Looping until last bit in the byte is reached.
    while (placeCounter < 8){

        // If remainder 0 then that bit is unbooked, proceed
        if (temp % 2 != 1){

            // toggle onBooking
            onBooking = 1;

            // tracking unbooked bits
            bitCounter++;

            // dropping the bit that i'm currently on
            temp = temp / 2;

            // keeping track of current place
            placeCounter++;

            // while current bit is unbooked, loop
            while (temp % 2 != 1){

                // break out of the loop if at the last bit in the byte is reached
                if (placeCounter == 8){
                    break;
                }

                // incrementing counter and dropping bit
                bitCounter++;
                temp = temp / 2;
                placeCounter++;
                }
        }

        // if not on a booking, proceed
        if (onBooking == 0){
            temp = temp / 2;
            placeCounter++;
        } else {
            // storing the values of the unbooked slot into the array
            unbookedArray[0][0] = 0;
            unbookedArray[0][1] = weekNum;
            unbookedArray[0][2] = byteCounter;
            unbookedArray[0][3] = placeCounter + 9 - bitCounter;
            unbookedArray[0][4] = bitCounter;

            // displaying those values and reseting counter.
            displayStudentExamSchedule(unbookedArray, 1);
            onBooking = onBooking - 1;
            bitCounter = bitCounter - bitCounter;
        }
    }
}
}


/*
This method takes the index of the first empty spot in the array and values of a new booking.
Then it inserts those values at the specified index in examBookings.
*/
void insertExamBooking(int index, int studentId, int examWeek, int examDay, int examStartTime, int examHours){

// printing success statement
printf("Timeblock was successfully booked.");

// insering booking into array
examBookings[index][0] = studentId;
examBookings[index][1] = examWeek;
examBookings[index][2] = examDay;
examBookings[index][3] = examStartTime;
examBookings[index][4] = examHours;


}