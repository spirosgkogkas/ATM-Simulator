#----------------Setup Information------------------
*For Windows Setup*
Download the ATM-Simulator.exe

*For Linux Users*
Run make and then make clean.
the executable will be in bin folder.
Example: make && make clean

#----------------Program Information----------------

Initial Menu:

0: Exit
1: Log in
2: Create an account

During the creation of an account, we cannot create
an account that already exists.

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
