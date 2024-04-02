#=========================================================================================================
# COURSEWORK 1: TASK 5
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
input_file:             .asciiz "task5.txt"
newline:                .asciiz "\n"
space:                  .asciiz " "
testflag:               .asciiz "!"

#---------------------------------------------------------------------------------------------------------
# GLOBAL VARIABLES IN MEMORY
#---------------------------------------------------------------------------------------------------------
buffer:                 .space 1000                     # A buffer used for file reading. The maxiumum
                                                        # text length is 1000 character,
                                                        # 999 characters or spaces, and one terminating
                                                        # newline.

# Your memory variables go here
word:                   .space 30    # Space to store a single word of max length 30 characters
words:                  .space 3000  # Space to store up to 100 unique words of max length 30 characters each
words_count:            .space 100     # Space to store the number of unique words
words_order:               .space 100     # Space to store the order of word
updated_words_order:       .space 100     # Space to store the order of word

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

        jal compress_more                               # call compress_more();

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

compress_more:
        # Implement me!
        la      $s0, buffer     # $s0 = address of buffer
        move    $t0, $s0       # $t0 = address of current char in buffer
        la      $s1, words      # $s1 = address of unique words
        la      $s2, word       # $s2 = address of word
        move    $t1, $s2      # $t1 = address of current char in word
        la      $s5, words_count        # $s5 = address of words_count
        la      $s6, words_order       # $s6 = address of words_id
        move    $s7, $s6       # $s7 = address of words_id
        li      $s3, 0        # $s3 = number of unique words

reading_char_loop:
        lb      $t4, 0($t0)                     # $t4 = current char
        beq     $t4, 0, finish_reading_buffer                  # if current char is null, finish_reading_buffer
        beq     $t4, 10, is_space               # if current char is newline,is_space
        beq     $t4, 32, is_space               # if current char is space, is_space


        sb      $t4, 0($t1)     # store current char in current word
        addi	$t0, $t0, 1	# increment address of current char in buffer
        addi    $t1, $t1, 1     # increment address of current char in current word

        j       reading_char_loop

is_space:
        # if current char is space, finish current word
        sb      $zero, 0($t1)     # store null in current word
        addi    $t0, $t0, 1      #      increment address of current char in buffer
        
        j       check_unique   # jump to check_unique

check_unique:
        # check if current word is unique
        move    $s4, $s1       # $s4 = address of current unique word
        move    $t2, $s4       # $t2 = address of char in current unique word
        move    $t3, $s2       # $t3 = address of char in current word
        li      $t7, 1         # $t7 = id of word

        beq     $s3, 0, store_unique_word     # if number of unique words is 0, store current word in unique words

check_current_char_loop:
        lb      $t6, 0($t2)     # $t6 = current char in current unique word
        lb      $t5, 0($t3)     # $t5 = current char in current word

        bne     $t6, $t5, check_next_unique_word      # if current char in both unique word and word is not same,
                                                # the word is not same, check next unique word
        beq     $t5, 0, next_word               # if current char in word is null, jump to next_word

        addi    $t2, $t2, 1     # increment address of current char in current unique word
        addi    $t3, $t3, 1  # increment address of current char in current word
        j       check_current_char_loop    # continue checking current char in both unique word and word

check_next_unique_word:
        addi    $s4, $s4, 30   # increment address of current unique word
        addi    $t7, $t7, 1     # increment number of word in unique words
        move    $t2, $s4       # $t2 = address of char in current unique word
        move    $t3, $s2       # $t3 = address of char in current word

        lb      $t6, 0($t2)     # $t6 = current char in current unique word
        beq     $t6, 0, store_unique_word    # if current char in current unique word is null, store current word in unique words

        j       check_current_char_loop      # continue checking current char in both unique word and word

store_unique_word:
       # store current word in unique words
        addi    $s3, $s3, 1     # increment number of unique words
        move    $t2, $s4       # $t2 = address of char in current unique word
        move    $t3, $s2       # $t3 = address of char in current word

store_word_loop:
        lb      $t6, 0($t3)     # $t6 = current char in current word
        sb      $t6, 0($t2)     # store current char in current unique word


        addi    $t2, $t2, 1     # increment address of current char in current unique word
        addi    $t3, $t3, 1     # increment address of current char in current word
        bne     $t6, 0, store_word_loop    # if current char in current word is not null, continue storing current word in unique words

next_word:
        # store the id of word in words_id
        sb      $t7, 0($s7)     # store id of word in words_id


       
        move    $t8, $s5      # $t8 = address of words_count
        add     $t8, $t8, $t7   # $t8 = address of words_count + number of word in unique words
        lb      $t9, 0($t8)     # $t9 = current words_count
        addi    $t9, $t9, 1     # increment words_count
        sb      $t9, 0($t8)     # store words_count


        move    $t1, $s2      # $t1 = address of current char in word
        addi    $s7, $s7, 1     # increment address of words_id
        j       reading_char_loop

finish_reading_buffer:
        
        la      $s0, words_order           # $s0 = address of words_id
        la      $s1, words_count        # $s1 = address of words_count
        la      $s2, words              # $s2 = address of unique words
        la      $s4, updated_words_order   # $s4 = address of updated_words_id
        move    $t0, $s0       # $t0 = address of current id of word in words_id
        li      $t1, 1          # $t1 = id of word in updated_words_id
iterate_order:
        lb      $t2, 0($t0)     # $t2 = current id of word in words_id
        beq     $t2, 0, finish_printing_compressed  # if current id of word in words_id is 0, finish_printing_compressed
        beq     $t1, 1, jump_print_first_space # if current id of word in updated_words_id is 1, jump to jump_print_first_space
        # print a space
        li      $v0, 4  # print a space
        la      $a0, space   # print a space
        syscall

jump_print_first_space:
        add     $t3, $s1, $t2   # $t3 = address of words_count + current id of word in words_id
        lb      $t4, 0($t3)     # $t4 = current words_count
        beq     $t4, 1, update_words_order  # if current words_count is 1, jump to update_words_id
        add     $t5, $s4, $t2  # $t5 = address of updated_words_id + current id of word in words_id
        lb      $t6, 0($t5)     # $t6 = current id of word in updated_words_id
        bne     $t6, 0, print_order  # if current id of word in updated_words_id is not 0, jump to print_id
        sb      $t1, 0($t5)     # store current id of word in updated_words_id
        move    $t6, $t1        # $t6 = current id of word in updated_words_id
        addi    $t1, $t1, 1    #        increment id of word in updated_words_id
print_order:
        # print the id of word in updated_words_id
        li      $v0, 1   # print the id of word in updated_words_id
        move    $a0, $t6    # print the id of word in updated_words_id
        syscall
        
        addi    $t0, $t0, 1     # increment index of words_id

        j       iterate_order   # continue iterating id of word in words_id

update_words_order:
        # update the id of words
        addi    $t0, $t0, 1    # increment index of words_id
        add     $t3, $t2, $s4   #       increment address of updated_words_id + current id of word in words_id
        li      $t4, -1         # $t4 = -1
        sb      $t4, 0($t3)     # store -1 in updated_words_id + current id of word in words_id
        addi    $t2, $t2, -1    # decrement current id of word in words_id
        li      $t5, 30        # $t5 = 30
        mult	$t5, $t2		# $t5 = 30 * current id of word in words_id
        mflo	$t5             # $t5 = 30 * current id of word in words_id
        add     $t5, $t5, $s2   # $t5 = address of unique words + 30 * current id of word in words_id

print_word:
  # print the word
        li      $v0, 4  # print the word
        move    $a0, $t5  # print the word
        syscall
        j       iterate_order   # continue iterating id of word in words_id


finish_printing_compressed:
        # print a space
        li      $v0, 4  # print a space
        la      $a0, space  #   print a space
        syscall

print_unique_words_num:
        li      $t0, 1       # $t0 = ordinal number of word in unique words
        move    $t1, $s2       # $t1 = address of current char in unique words
        move    $t4, $s1      # $t4 = address of words_count
        addi    $t1, $t1, -30           # $t1 = address of current char in unique words

print_unique_words_num_loop:
        addi    $t1, $t1, 30    # increment address of current char in unique words
        addi    $t4, $t4, 1     # increment address of current words_count
        lb      $t2, 0($t4)    # $t2 = current words_count
        beq     $t2, 0, finish_compress_more   # if current words_count is 0, finish_compress_more
        beq     $t2, 1, print_unique_words_num_loop # if current words_count is 1, continue printing unique words dict

        # Print the next line
        li      $v0, 4  # print the next line
        la      $a0, newline  # print the next line
        syscall

     
        li      $v0, 1  # print the ordinal number of word in unique words
        move    $a0, $t0   # print the ordinal number of word in unique words
        syscall
        # print space
        li      $v0, 4   # print space
        la      $a0, space  # print space
        syscall

        addi    $t0, $t0, 1    # increment ordinal number of word in unique words

        move    $t3, $t1      # $t3 = address of current char in unique words

print_unique_words_num_char_loop:
        li      $v0, 4    # print current char in unique words
        move    $a0, $t3   # print current char in unique words
        syscall
        j       print_unique_words_num_loop    # continue printing unique words dict

finish_compress_more:

        # Print the next line
        li      $v0, 4    # print the next line
        la      $a0, newline    # print the next line  

        syscall  # print the next line

        jr      $ra