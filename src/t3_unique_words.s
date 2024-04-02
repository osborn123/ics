#=========================================================================================================
# COURSEWORK 1: TASK 3
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
input_file:             .asciiz "task3.txt"
newline:                .asciiz "\n"
space:                  .asciiz " "

#---------------------------------------------------------------------------------------------------------
# GLOBAL VARIABLES IN MEMORY
#---------------------------------------------------------------------------------------------------------
buffer:                 .space 1000 
words:      .space 1000             # Assume a maximum of 1000 characters for unique words
word:       .space 30               # Each word is a maximum of 30 characters


                    # A buffer used for file reading. The maxiumum
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

        jal unique_words                                # call unique_words();

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
# PRINT UNIQUE FUNCTION
#---------------------------------------------------------------------------------------------------------


unique_words:
    # Implement me!
    # Initialize registers
    la      $s0, buffer         # s0 = address of buffer
    move    $t0, $s0          # t0 = current position in buffer
    la  $s1, words           # s2 = address of word
    la  $s2, word          # s1 = address of unique words
    move    $t2, $s2    # t2 = current position in word
    li  $s3, 0              # s3 = number of unique words
    

reading_char:
    lb      $t3, 0($t0)         # t3 = current char
    beq     $t3, 0, program_end  # if current char is null, finish
    beq     $t3, 10, is_space       # check for space
    beq     $t3, 32, is_space       # check for newline

    # Store char in word
    sb  $t3, 0($t2)
    addi    $t0, $t0, 1   # move next char
    addi    $t2, $t2, 1  # move next char

    j reading_char

is_space:
    sb  $zero, 0($t2)       # store null in word
    addi    $t0, $t0,1  # move next char
    addi    $t2,$t2,1   # move next char
    j   check_unique


check_unique:
    move   $s4,$s1      #s4 = address of current unique word
    move    $t1,$s4     #t1 = address of current unique word
    move    $t4,$s2     #s2= address of current word
    beq     $s3,0,store_unique_word  #if now unique word, store word

check_current_char:
    lb      $t6,0($t1)      #t6 = current char of unique word
    lb      $t5,0($t4)      #t5 = current char of word
    bne     $t6,$t5,check_next_unique_word         #check unique word and current word is equal
    beq     $t5,0,next_word #if current char is null,check next word

    addi    $t1,$t1,1   #move next unique word
    addi    $t4,$t4,1   #mobe next word

    j       check_current_char
check_next_unique_word:
    addi    $s4,$s4,30      #increase address for unique word
    move    $t1,$s4         #t1 = address of current unique word
    move    $t4,$s2         #t4 = address of current word
    lb      $t6,0($t1)      #t6 = load current current char in unique word
    beq     $t6,0,store_unique_word
    j   check_current_char

store_unique_word:
    addi  $s3,$s3,1     #increase number for uniqur word
    move    $t1,$s4     #t1 = address of current unique word
    move    $t4,$s2      #t4 = addredss of current word

    #print count for unique word
    li      $v0,1
    move    $a0,$s3
    syscall

    #print a space
    li  $v0,4
    la  $a0,space
    syscall
store_word_loop:
    lb  $t6,0($t4)   #t6 = load current char in current word
    sb  $t6,0($t1)  #store current char in unique word
    beq     $t6,0,print_null     #check if it is null, finish 

    li      $v0,11  #print current char
    move    $a0,$t6  #  print current char
    syscall

    addi    $t1,$t1,1
    addi    $t4,$t4,1 #move next char for both unique and current word
    bne     $t6,0,store_word_loop # if not end keep store word
print_null:
    li  $v0,4
    la  $a0,newline
    syscall
    # print new line

next_word:  
    move    $t2,$s2
    j   reading_char
program_end:
    # Print a newline at the end
    li  $v0, 4
    la  $a0, newline
    syscall

    jr $ra