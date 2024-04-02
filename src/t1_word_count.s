#=========================================================================================================
# COURSEWORK 1: TASK 1
#=========================================================================================================
# Inf2C Computer Systems
# David Schall
# 6 October 2023

#=========================================================================================================
# DATA SEGMENT
#=========================================================================================================
.data

#---------------------------------------------------------------------------------------------------------
# CONSTANT STRINGS
#---------------------------------------------------------------------------------------------------------
input_file:             .asciiz "task1.txt"
newline:                .asciiz "\n"
space:                  .asciiz " "

#---------------------------------------------------------------------------------------------------------
# GLOBAL VARIABLES IN MEMORY
#---------------------------------------------------------------------------------------------------------
buffer:                 .space 1000                     # A buffer used for file reading. The maxiumum
                                                        # text length is 1000 character,
                                                        # 999 characters or spaces, and one terminating
                                                        # newline.

# Your memory variables go here


#=========================================================================================================
# TEXT SEGMENT
#=========================================================================================================
.text


#---------------------------------------------------------------------------------------------------------
# MAIN CODE BLOCK (DO NOT MODIFY)
#---------------------------------------------------------------------------------------------------------
.globl main                                             # Declare the main label as globally visible
main:                                                   # This is where the program will start executing

#---------------------------------------------------------------------------------------------------------
# READ INTEGER FILE FUNCTION (DO NOT MODIFY)
#---------------------------------------------------------------------------------------------------------
read_file:
        li $v0, 13                                      # Load the system call code for open file
        la $a0, input_file                              # Load the address of the input file name
        li $a1, 0                                       # Flag for reading
        li $a2, 0                                       # Mode is ignored
        syscall

        move $s0, $v0                                   # Save the file descriptor returned by syscall

	li $v0, 14                                      # Load the system call code for reading from file
        move $a0, $s0                                   # Load the file descriptor to read
        la $a1, buffer                                  # Load the address of the buffer to write into
        li $a2, 1000                                    # Read the entire file into the buffer
        syscall


        li $v0, 16                                      # Load the system call for close file
        move $a0, $s0                                   # Load the file descriptor to close
        syscall

        jal word_count                                	# word_count();

#---------------------------------------------------------------------------------------------------------
# EXIT PROGRAM (DO NOT MODIFY)
#---------------------------------------------------------------------------------------------------------
main_end:
        li $v0, 10
        syscall

#---------------------------------------------------------------------------------------------------------
# END OF MAIN CODE BLOCK
#---------------------------------------------------------------------------------------------------------

# All below is yours ;-)

#---------------------------------------------------------------------------------------------------------
# WORD COUNT FUNCTION
#---------------------------------------------------------------------------------------------------------
word_count:
        # Implement me!
        la $t0,buffer   #laod t0 to address buffer which is inpute string
        li $t1,0        # load 0 to t1 refer to word count
        li $t2 ,0       #load 0 to t2 refer to current state
read_character:
        lb $t3,($t0)                    #load a byte from buffer to t3
        
        beq $t3,10,finish_count         #if is newline to finish
        beq  $t3,32, check_state         #if is space to add one
        
     
        beq $t2, 1, reading
      addi    $t1, $t1, 1       # Increment word count
      li $t2, 1
 reading:
        
       
       addi    $t0, $t0, 1       # Increment address to the next character
      
       j read_character

check_state:
        beq $t2,0,reading
        li $t2,0
        j reading
 finish_count:
   # Prepare print
    move $a0, $t1 # move the word count to $a0 for printing
    li $v0, 1 #print int
    syscall # print the integer

        li $v0, 11  #print char
    li $a0, '\n'  # newline character
    syscall # print the newline characters
    jr $ra




