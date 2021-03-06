
//
//  ColumnarTransposition.cpp
//  Document Encryptor
//
//  Created by Akshay Subramaniam on 25/5/15.
//  Copyright (c) 2015 Incultus. All rights reserved.
//

#include "ColumnarTransposition.h"

// This class uses the columnar transposition cipher to encrypt and decrypt the message. This constructor acquires the message that needs to be encrypted/decrypted, establishes whether to encrypt or decrypt, and determines whether this class is used for the first, or second encryption with the columnar transposition cipher.
ColumnarTransposition::ColumnarTransposition(std::string message, bool encrypt, bool single) : Cipher(message), single(single) {
    if (encrypt)
        getCipherText();
    else
        getPlainText();
}

// Method that creates the cipher text for encryption.
void ColumnarTransposition::getCipherText() {
    // Splits the message up into smaller messages of length 25.
    std::vector<std::string> assortedMessage = getMessageAssorted();
    // Encrypts each broken-up message, and then adds it to the cipher text.
    for (std::vector<std::string>::iterator it = assortedMessage.begin(); it != assortedMessage.end(); ++it)
        cipherText += getCipherTextPart(getCipherMessageMatrix(*it));
    
    // If this class is used for the second encryption, it creates the spacing that allows the cipher text to be later decrypted.
    if (!single) {
        int count = 0;
        for (int i = 0; i < cipherText.length(); ++ i)
            if ((i - count) % 4 == 0 && i != 0) {
                cipherText.insert(i, " ");
                ++ count;
                ++ i;
            }
    }
}

// Method that creates the plain text for decryption.
void ColumnarTransposition::getPlainText() {
    // Functions the same as the encrypted version.
    std::vector<std::string> assortedMessage = getMessageAssorted();
    for (std::vector<std::string>::iterator it = assortedMessage.begin(); it != assortedMessage.end(); ++it)
        plainText += getPlainTextPart(getPlainMessageMatrix(*it));
    
    // Gets rid of the extra characters that may have appended due to the last segment being too short.
    for (int i = plainText.length(); i > -1; i --)
        // Gets the position of the * symbol, if it exists.
        if (plainText.substr(i, 1) == "*") {
            // Gets the length of the characters after the * symbol.
            int length = plainText.substr(i + 1, length).length();
            if (length < 6) {
                // If the length is less than six, checks if the first x letters of fedcba match up with the characters after the * symbol, where x is the length.
                if (plainText.substr(i + 1, length) == std::string("fedcba").substr(0, length))
                    // Truncates the string to the * point if so.
                    plainText = plainText.substr(0, i);
            }
            // Else if the letters immediately following the * are fedcba, truncates the string to the * point.
            else if (plainText.substr(i + 1, 6) == "fedcba")
                plainText = plainText.substr(0, i);
        }
}

// Creates the vector of strings that represents the assorted message (strings of three characters).
std::vector<std::string> ColumnarTransposition::getMessageAssorted() {
    std::vector<std::string> assortedMessage;
    
    //Checks if the message length is a multiple of 25. If not creates "junk" letters.
    if (message.length() % 25 != 0) {
        // Gets the number of characters away from making the message length a multiple of 25.
        int junkLetters = 25 - message.length() % 25;
        // Creates a * symbol to serve as a marker for the decryptor to recognize.
        message += "*";
        for (int i = 1; i < junkLetters; i ++) {
            //Adds additional junk letters, depending on the position of the character. Creates a variation fedcba multiple times.
            if (i % 6 == 0)
                message += "a";
            else if (i % 5 == 0)
                message += "b";
            else if (i % 4 == 0)
                message += "c";
            else if (i % 3 == 0)
                message += "d";
            else if (i % 2 == 0)
                message += "e";
            else
                message += "f";
        }
    }
    
    // Creates the vector of assorted strings, by pushing back the larger message in bits of 25.
    for (int i = 0; i < message.length(); i += 25)
        assortedMessage.push_back(message.substr(i, 25));
    
    return assortedMessage;
}

// Gets a vector of 5x5 matrices, which represent each part of the assorted message put into a 5x5 matrix, row by row.
std::vector<std::vector<std::string>> ColumnarTransposition::getCipherMessageMatrix(std::string assortedMessage) {
    std::vector<std::vector<std::string>> messageMatrix;
    // creates the 5x5 matrix.
    messageMatrix.resize(6);
    for (int i = 0; i < 6; ++i)
        messageMatrix[i].resize(6);
    
    // If the class serves as the first encryption, it creates these list of keys at the top of the matrix. Each number represents the order in which the columns are added to the cipher text. How the cipher works must be understood to comprehend this. So, column 3 would be read first, and column 1 last.
    if (single) {
        messageMatrix[0][0] = "6";
        messageMatrix[0][1] = "4";
        messageMatrix[0][2] = "1";
        messageMatrix[0][3] = "5";
        messageMatrix[0][4] = "3";
        messageMatrix[0][5] = "2";
    }
    
    // Same but if the class serves as the second level of encryption.
    else {
        messageMatrix[0][0] = "3";
        messageMatrix[0][1] = "1";
        messageMatrix[0][2] = "5";
        messageMatrix[0][3] = "2";
        messageMatrix[0][4] = "6";
        messageMatrix[0][5] = "4";
    }
    
    int count = 0;
    
    // Adds each character of the assorted message, from 0 to 24, left to right. So if the matrix is read left to right, top to bottom (skipping the first row), the message fragment can be read. Note the very last letter of the fragment is NOT added. Note also that the matrix as of this point is really 4x4, not 5x5.
    for (int x = 1; x < 5; x ++)
        for (int y = 0; y < 6; y ++) {
            messageMatrix[x][y] = assortedMessage.substr(count, 1);
            count ++;
        }
    
    // Adds the last, dangling letter of the matrix. This actually creates an incomplete 5x5 matrix, with 4 characters missing from the bottom. This last letter is added to the column that is read last, so that the cipher reads correctly.
    for (int y = 0; y < 6; y ++)
        if (messageMatrix[0][y] == "6")
            messageMatrix[5][y] = assortedMessage.substr(count, 1);
    
    return messageMatrix;
}

// Largely the same as the encrypted version.
std::vector<std::vector<std::string>> ColumnarTransposition::getPlainMessageMatrix(std::string assortedMessage) {
    std::vector<std::vector<std::string>> messageMatrix;
    messageMatrix.resize(6);
    for (int i = 0; i < 6; ++i)
        messageMatrix[i].resize(6);
    
    if (single) {
        messageMatrix[0][0] = "6";
        messageMatrix[0][1] = "4";
        messageMatrix[0][2] = "1";
        messageMatrix[0][3] = "5";
        messageMatrix[0][4] = "3";
        messageMatrix[0][5] = "2";
    }
    
    else {
        messageMatrix[0][0] = "3";
        messageMatrix[0][1] = "1";
        messageMatrix[0][2] = "5";
        messageMatrix[0][3] = "2";
        messageMatrix[0][4] = "6";
        messageMatrix[0][5] = "4";
    }
    
    int count = 0;
    
    // This is the reverse way of the encrypted version, and actually adds the assorted message fragments in a columnar fashion, so that the matrix is read top to bottom, left to right. This reverses the process of the method used to encrypt the matrix.
    for (int i = 1; i < 7; i ++)
        for (int y = 0; y < 6; y ++)
            // Finds the first column that was encrypted, and adds the text accordingly in a vertical fashion.
            if (messageMatrix[0][y] == std::to_string(i))
                for (int x = 1; x < 5; x ++) {
                    messageMatrix[x][y] = assortedMessage.substr(count, 1);
                    count ++;
                }
    
    // Adds the dangling letter in the same way as the encrypted version.
    for (int y = 0; y < 6; y ++)
        if (messageMatrix[0][y] == "6")
            messageMatrix[5][y] = assortedMessage.substr(count, 1);
    
    return messageMatrix;
}

// This method creates the cipher text by reading the 5x5 matrix vertically, in the order set by the keys.
std::string ColumnarTransposition::getCipherTextPart(std::vector<std::vector<std::string>> messageMatrix) {
    std::string cipherTextPart = "";
    for (int i = 1; i < 5; i ++) {
        for (int y = 0; y < 6; y ++)
            // Finds the key number corresponding to i, which goes from 1 to 6.
            if (messageMatrix[0][y] == std::to_string(i))
                // Then adds the character to the ciphertext in the column number. The number of characters added gets progressively less (from 4 to 3 to 2 and so on).
                for (int x = i; x < 5; x ++)
                    cipherTextPart += messageMatrix[x][y];
        // Adds the corresponding amount of characters from the next column to the cipher text. The number of characters added gets progressively more.
        for (int y = 0; y < 6; y ++)
            if (messageMatrix[0][y] == std::to_string(i + 1))
                for (int x = 1; x < i + 1; x ++)
                    cipherTextPart += messageMatrix[x][y];
    }
    
    // Adds the last, dangling letter to the cipher text.
    for (int y = 0; y < 6; y ++)
        if (messageMatrix[0][y] == "6")
            for (int x = 1; x < 6; x ++)
                cipherTextPart += messageMatrix[x][y];
    
    // Returns the fully created cipher text.
    return cipherTextPart;
}

// The much simpler version for decryption.
std::string ColumnarTransposition::getPlainTextPart(std::vector<std::vector<std::string>> messageMatrix) {
    std::string plainTextPart = "";
    
    // Simply horizontally adds the matrix characters to the plain text. Left to right, top to bottom.
    for (int x = 1; x < 6; x ++) {
        for (int y = 0; y < 6; y ++) {
            plainTextPart += messageMatrix[x][y];
        }
    }
    
    // Returns the fully created plain text.
    return plainTextPart;
}