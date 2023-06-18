# ATM-Simulator
ATM-Simulator is a computer-based program replicating the basic functionality of an ATM.

## Setup Information

*For Windows Setup*

Download the ATM-Simulator.exe

*For Linux Users*

Run `make && make clean` to compile the source code.
> The executable will be in the bin folder.

# How the app works

0. Exit
   * Exit the program.
1. Log in
   - To log in you should have a unique bank ID, if the bank ID is invalid you will be asked again
     until you provide an existing bank ID or enter Q or Q.
2. Register
   - [Bank-ID] To register you should provide a unique ID if the ID already exists you will get redirected back to the main menu.
   - [PIN] You can provide anything for the Pin at the moment 

**User Menu**

1: Deposit
The user has the option to cancel the transaction.

2: Withdrawal
The user has the option to cancel the transaction
and is not allowed to withdraw more money than they already have.

3: Transfer
Transfer money from the current account to another if it exists.
If the user enters the current account as an option,
it informs them and rejects the transaction.

4: Last transactions
Prints the last 5 transactions (if any), and if
there are no transactions, it notifies the user.

5: Account information
Prints the user's balance and their name.

6: Change password
The user has the option to cancel the change.

7: Exit
Returns to the initial menu.
