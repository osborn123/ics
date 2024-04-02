#=========================================================================================================
# COURSEWORK 1: TASK 6
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
input_file:             .asciiz "task6.txt"
newline:                .asciiz "\n"
space:                  .asciiz " "
alphabet:   .asciiz "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
# Frequency map with 26 characters (2 bytes each) plus space for null character and newline


#---------------------------------------------------------------------------------------------------------
# GLOBAL VARIABLES IN MEMORY
#---------------------------------------------------------------------------------------------------------
buffer:                 .space 1000                     # A buffer used for file reading. The maxiumum
                                                        # text length is 1000 character,
                                                        # 999 characters or spaces, and one terminating
                                                        # newline.
.align 2
char_array:   .space 104
one_char: .space 1

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

        jal huffman                                     # call huffman();

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
# HUFFMAN ENCODING FUNCTION
#---------------------------------------------------------------------------------------------------------

huffman:
 
#   la $t0,  buffer  #load buffer into t0
  
  
  
#   la $t1,  char_array  #load char_array into t1
#   la $t2 , alphabet  # load alphabet into t2
#   la $s4 ,space    #load space and newline into s4 and s5
#   la $s5,newline   #load space and newline into s4 and s5

#   lb $s0, 0($s4)  #load space and newline into s4 and s5
#   lb $s1, 0($s5)  #load space and newline into s4 and s5

#   move $t3, $t2    #move alphabet into t3
#   move $t4, $t1    #move char_array into t4

# count_loop:
#     lb $t5, 0($t0)   #load buffer into t8
#     lb $t6, 0($t1)    #load char_array into t7
#     beq $t5, $s1, sort    #if t8 is equal to newline, jump to check_char
#     beq $t5, $t6, increase_count    #if t8 is equal to t7, jump to increase_count
    
#     addi $t3 , $t3 , 1   #add 1 to t3
#     addi $t4, $t4 , 1    #add 1 to t4
#     j count_loop

# increase_count:
#       lw $t8, 0($t4)    #load char_array into t8
#       addi $t8, $t8, 1   #add 1 to t8
#       sw $t8, 0($t4)     #store t8 into char_array

#       addi $t0, $t0, 1   #mobe next character in buffer
#       move $t3, $t2    #move alphabet into t3
#       move $4, $t1     #move char_array into t4

#       j count_loop #jump to count_loop
# sort:
#      li $s7,26  #load 26 into s7
#      la $t1,alphabet  #load alphabet into t1
#      la $t0,char_array  #load char_array into t0
#      la $s6 ,one_char   #load one_char into s6
#      li $s5,0   #load 0 into s5
#      move $t2,$t0  #move char_array into t2
#      move $t3,$t1  #move alphabet into t3
# sort_loop:
#      lw $t4,0($t2) #load alphabet into t4
#      beq $t4,0, next_char
#      bgt $t4,$s5, increase_counting  #if t4 is greater than s5, jump to increase_count
#      j next_char
# next_char:
#       addi $t2,$t2,4  #add 4 to t2
#       addi $t3,$t3,1  #add 1 to t3
#      lb $t8,0($t3)  #load alphabet into t45    
#      beq $t8,0,print  #if t8 is equal to newline, jump to sort_loop  
     
#      j sort_loop  #jump to sort_loop
# increase_counting:
#       move $s5,$t4  #move t4 into s5
#       move $t9,$t2  #move t2 into t9

#       lb $t8,0($t3)  #load alphabet into t7
#       sb $t8,0($s6)  #store t7 into s6
#       j next_char  #jump to next_char
# print:
#       lw $t5,0($t9)  #load char_array into t5
#       beq $t5,0,end #if t5 is equal to newline, jump to sort_loop
#       li $t5, 0  #load 11 into v0
#       sw $t5, 0($t9)  #store t5 into char_array


#         li $v0, 11  #load 11 into v0
#         lb $a0, 0($s6)  #load s6 into a0
#         syscall  #print character

#         li $v0,4
#         la $a0,space
#         syscall

#         li $v0,1
#         move $a0,$s5
#         syscall

#         li $v0,4
#         la $a0,newline
#         syscall
        

#     li $s5,0
#     addi $s7,$s7,-1
#     beq $s7,$zero,end
#     j sort_loop
# end:
#     jr $ra






      



 

  