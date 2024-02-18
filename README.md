# WordChecker-String-Manipulation-Algorithm
Polytechnic of Milan - Prof. Pradella - Algorthms and Data Structures<br><br>
Final Grade: 30 Cum Laude / 30<br><br>
The aim of this project is to study an algorithm, with appropiate data structure, in order to execute various string analysis operations. The motivation is to implement an advanced version of the famous game Wordle, as descibed in the specifications. The code is entirly written in Ansi C 89.<br><br>
The approach I used was to use various hash maps, implemented as matrices of characters, for modeling the constrains and a binary search tree (BST) to store the dictionary of words. When the dictionary shirks down to a certain percentage the BST is automatically deleted and a Linked List is built, with the remaining words (this helps in reducing the overall space complexity). The linked list often mutate, and every time it happens the program sorts it with merge sort algotithm for linked list (this helps in reducing the overall time complexity).<br><br>
Finally, I have also provided an alternative version using the Trie data structure, which is the most efficent data structure for storing strings (in terms of time complexity). Obviously it's much faster but it occupies much more space and that's why I don't recommend it, for this specific case. 
