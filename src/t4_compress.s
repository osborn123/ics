#=========================================================================================================
# COURSEWORK 1: TASK 4
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
input_file:             .asciiz "task4.txt"
newline:                .asciiz "\n"
space:                  .asciiz " "

#---------------------------------------------------------------------------------------------------------
# GLOBAL VARIABLES IN MEMORY
#---------------------------------------------------------------------------------------------------------
buffer:                 .space 1000                     # A buffer used for file reading. The maxiumum
                                                        # text length is 1000 character,
                                                        # 999 characters or spaces, and one terminating
                                                        # newline.

unique_list:           .space 3100                      

word:            .space 31                      

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

        jal unique_words                                # call compress();

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
# COMPRESS FUNCTION
#---------------------------------------------------------------------------------------------------------

unique_words:
     # Implement me!
    
    la $t0, buffer                 # t0 = base address of buffer
    la $t1, unique_list             #t1 = base address of unique_List
    la $t2, word         # t2 = base address of unique_buffer                         
    li $t3,0                       # t3 = counter for unique_List
    la $s4, space                # s4 =base address of space into
    la $s5, newline                     # t5 =base address of newline 
                                                        
    lb $s0, 0($s4)                 # s0 =first character of space 
    lb $s1, 0($s5)                 # s1 = irst character of newline 
    li $s5,0                       # t5 = counter for unique_buffer
    move $s2, $t1                 # s2 = move  base address of unique_List 
    move $s3, $t2               # s3 = move base address of unique_buffer

check_current_word:
        # check if the word is end
        lb $t4, 0($t0)                                  # t3 = the next character of buffer

        # check if the character is space or newline             
        beq $t4, $s0, check_uni            # if the character is space, verify_unique                         
        beq $t4, $s1, check_uni          # if the character is newline,verify_unique
        beq $t4, $zero, programme_end        # if the character is zero, program_end

        # otherwise, store the character into unique_buffer
        sb $t4, 0($s3)                    # store the character into unique_buffer
        addi $t0, $t0, 1                  # move to the next character of buffer
        addi $s5, $s5, 1               # move to the next position of unique_buffer
        addi $s3,$s3,1                   # move to the next position of unique_buffer
        j check_current_word                     # repeat the loop


check_uni:
        # check if the word is unique
        addi $s3, $s3, 1                 # move to the next position of unique_buffer
        sb $zero, 0($s3)         # initial is null for word in unique_buffer

        move $s3, $t2                    # reset the unique_buffer
        move $t4, $t1                     # reset the unique_List
        move $t5, $t2                   # reset the unique_buffer

check_loop:
    li $t9,0        # t9 = counter for unique_List
loop_check:   
    lb $t6, 0($t4)                                  # t3 = the next character of unique_List
    lb $t7, 0($t5)                                  # t5 = the next character of unique_buffer
    
    bne $t6,$t7,next_unique_word # if the character is not equal, go to next_uni_word
    beqz $t6,clear_buffer # if the character is zero, clear_buffer
    
    addi $t4, $t4, 1                  # move to the next character of unique_List
    addi $t5, $t5, 1                  # move to the next character of unique_buffer
    j loop_check           # repeat the loop
next_unique_word:
    addi $t9, $t9, 1                  # move to the next character of unique_List
    move $t4, $t1                     # reset the unique_List
    mul $t7, $t9, 31          # t7 = t9 * 31
    add $t4, $t4, $t7          # t4 = t4 + t7
    lb $t8, 0($t4)                                  # t8 = the next character of unique_List
    beqz $t8,save_word # if the character is zero, saving_word
    move $t5,$t2                  # reset the unique_buffer
    j loop_check           # repeat the loop
save_word:
    addi $t3, $t3, 1                  # move to the next character of buffer
    move $t4, $t1                     # reset the unique_List
    addi $t6,$t3,-1                 # t6 = t3 - 1
    mul  $t5,$t6,31         # t5 = t6 * 31
    add $t4, $t4, $t5          # t4 = t4 + t5
    move $t7,$t2                 # reset the unique_buffer

saving_loop:
    lb $t6,0($t7) # t6 = the next character of unique_buffer
    beqz $t6,clear_buffer # if the character is zero, clear_buffer
    sb $t6, 0($t4)                    # store the character into unique_List

    addi $t4, $t4, 1                  # move to the next character of unique_List
    addi $t7, $t7, 1                  # move to the next character of unique_buffer
    j   saving_loop # repeat the loop
clear_buffer:
    addi $t4, $t4, 1                  # move to the next character of unique_List
    sb $zero, 0($t4)                    # store the character into unique_List
    move $t4,$t2                 # reset the unique_buffer
clear_loop:
    lb $t5,0($t4) # t6 = the next character of unique_buffer
    beqz $t5,next_word # if the character is zero, next_word
    sb $zero, 0($t4)                    # store the character into unique_List
    addi $t4, $t4, 1                  # move to the next character of unique_List
    j clear_loop # repeat the loop
next_word:
    lb $t4,0($t0) # t4 = the next character of buffer
    bne $t4,$s0,compress # if the character is not equal, check_word
    move $t5,$t2                 # reset the unique_buffer
    
    move $s2,$t1                 # reset the unique_List
    addi $t6,$t3,-1                 # t6 = t3 - 1
    mul  $t7,$t6,31         # t5 = t6 * 31
    add $s2, $s2, $t7          # t4 = t4 + t5
    addi $t0, $t0, 1                  # move to the next character of buffer
    addi $s5,$s5,1                   # move to the next position of unique_buffer
    addi $s2, $s2, 31                  # move to the next character of buffer
    j check_current_word                     # repeat the loop
compress:
    li $t3,1                       # t3 = counter for unique_List
    la $t1,unique_list           # t1 = base address of unique_List
    la $t0,buffer                  # t0 = base address of buffer
    li $s6,0                       # s6 = counter for unique_List
    move $t7,$t0                   # reset the unique_buffer
    move $t9,$t1                   # reset the unique_List
compress_loop:
    bgt $s6,$s5,programme_end        # if the character is not equal, end
    lb $t4,0($t7) # t4 = the next character of buffer
    lb $t5,0($t9) # t5 = the next character of unique_List
    beq $t5,$zero,print_order # if the character is zero, print_index
    bne $t4,$t5,check # if the character is not equal, check
    beq $t4,$s0,print_order # if the character is space, print_index
    beq $t4,$s1,print_order # if the character is newline, print_index

    addi $t7, $t7, 1                  # move to the next character of buffer
    addi $s6, $s6, 1                  # move to the next character of unique_List
    addi $t9,$t9,1                   # move to the next position of unique_buffer
    j compress_loop # repeat the loop
increase_buffer_order:
    addi $t7, $t7, 1                  # move to the next character of buffer
    addi $s6,$s6,1                   # move to the next position of unique_buffer
    j compress_loop # repeat the loop
check:
    beq $t4,$s0,print_order

next_compress:
    addi $t3, $t3, 1                  # move to the next character of unique_List
    bgt $t3,101,programme_end       # if the character is not equal, end
    move $t9,$t1                   # reset the unique_List
    addi $t8,$t3,-1                 # t6 = t3 - 1
    mul $t4,$t8,31         # t5 = t6 * 31
    add $t9, $t9, $t4          # t4 = t4 + t5
word_begin:
    lb $t5,0($t7)# t5 = the next character of buffer
    beq $t5,$s0,increase_buffer_order # if the character is space, increasing_buffer_index
    beq $t7,$t0,compress_loop # if the character is newline, compress_loop
    addi $t7, $t7, -1             # move to the previous character of buffer
    addi $s6,$s6,-1                   # move to the previous position of unique_buffer
    j word_begin # repeat the loop
compare_next:
    addi $t3, $t3, 1                  # move to the next character of unique_List
    bgt $t3,101,programme_end       # if the character is not equal 101, end
    move $t9,$t1                   # reset the unique_List
    addi $t8,$t3,-1                 # t6 = t3 - 1
    mul $t4,$t8,31         # t5 = t6 * 31
    add $t9, $t9, $t4          # t4 = t4 + t5
    lb $t5,0($t7)# t5 = the next character of unique_List
    addi $t5, $t5, -1       # move to the previous character of buffer
    beq $t5,$s0,increase_buffer_order # if the character is space, increasing_buffer_index
    beq $t7,$t0,compress_loop # if the character is newline, compress_loop
    addi $t7, $t7, -1             # move to the previous character of buffer
    addi $s6,$s6,-1                   # move to the previous position of unique_buffer
    j word_begin # repeat the loop
print_order:
    bne $t5,$zero,compare_next# if the character is not equal, compare_next_word
    beq $t4,$s1,print  # if the character is newline, print
    bne $t4,$s0,compare_next  # if the character is not equal, print
print:
    li $v0, 1 # system call for print_int
    move $a0, $t3 # argument: int to print
    syscall # print the integer

    li $t3,1                       # t3 = counter for unique_List
    beq $t4,$s0,print_space # if the character is space, print_space
    beq $t4,$s1,print_nextline # if the character is newline, print_newline
print_space:
    li $v0, 4 # system call for print_int
    la $a0, space # argument: int to print
    syscall # print the integer
    
    addi $t7, $t7, 1                  # move to the next character of buffer
    addi $s6,$s6,1                   # move to the next position of unique_buffer
    move $t9,$t1                   # reset the unique_List
    j compress_loop # repeat the loop
print_nextline:
    li $v0, 4 # system call for print_int
    la $a0, newline # argument: int to print
    syscall # print the integer
    
programme_end:
   
    jr   $ra # return to main