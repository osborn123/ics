#=========================================================================================================
# COURSEWORK 1: TASK 2
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
input_file:             .asciiz "task2.txt"
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

        jal print_words                                	# call print_words();

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
# PRINT WORDS FUNCTION
#---------------------------------------------------------------------------------------------------------

print_words:
        # Implement me!
        la $t0,buffer   #load the address of buffer into $t0
print_loop:    
        lb $t1,0($t0)                   #laod byte from buffer to t1
        beq $t1,'\n',end_function       #check for end of function when word end

print_word:
        lb $t1,0($t0)                   #laod print char
        beq $t1,'\n',end_function       #check for end of function when word end
        beq $t1,' ',print_newline            #check for whitespace to get new line
        li  $v0,11                      #print a char
        move $a0, $t1                   #move a char to a0
        syscall
        addi $t0,$t0,1                  #make buffer change to next char
        j print_word
print_newline:
        li $v0,4                        #for print a string
        la $a0,newline                  #make a newline
        syscall
        addi $t0,$t0,1  
        j print_loop                    #return to main loop
end_function:
        jr $ra




