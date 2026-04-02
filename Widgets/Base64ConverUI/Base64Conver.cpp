// /*
//  * @Author: Sordayne 455248805@qq.com
//  * @FilePath: \Base64UI\src\Base64Conver.cpp
//  */
#pragma once
#include "Base64Conver.h"
#include <fstream>
#include <vector>
#include <algorithm>

// Base64编码表
const std::string base64Chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

// 解码文件
bool Base64Conver::encFile(fs::path inputFile, fs::path outputFile, std::function<void*(size_t, size_t)> callback) {
    std::ifstream ifs(inputFile, std::ios::binary);
    if (!ifs.is_open()) {
        return false;
    }
    
    std::ofstream ofs(outputFile);
    if (!ofs.is_open()) {
        return false;
    }
    
    ifs.seekg(0, std::ios::end);
    size_t fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    
    const size_t bufferSize = 3072;
    std::vector<char> buffer(bufferSize);
    size_t totalProcessed = 0;
    
    while (ifs.good() && !ifs.eof()) {
        ifs.read(buffer.data(), bufferSize);
        size_t bytesRead = ifs.gcount();
        
        if (bytesRead > 0) {
            std::string encoded = encode(reinterpret_cast<const unsigned char*>(buffer.data()), bytesRead);
            ofs.write(encoded.c_str(), encoded.length());
            
            totalProcessed += bytesRead;
            
            if (callback) {
                callback(fileSize, totalProcessed);
            }
        }
    }
    
    ifs.close();
    ofs.close();
    return true;
}

// 编码文件
bool Base64Conver::decFile(fs::path inputFile, fs::path outputFile, std::function<void*(size_t, size_t)> callback) {
    std::ifstream ifs(inputFile, std::ios::binary);
    if (!ifs.is_open()) {
        return false;
    }
    
    std::ofstream ofs(outputFile, std::ios::binary);
    if (!ofs.is_open()) {
        return false;
    }
    
    ifs.seekg(0, std::ios::end);
    size_t fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    
    const size_t bufferSize = 4096;
    std::vector<char> buffer(bufferSize);
    std::string decodedBuffer;
    size_t totalProcessed = 0;
    
    while (ifs.good() && !ifs.eof()) {
        ifs.read(buffer.data(), bufferSize);
        size_t bytesRead = ifs.gcount();
        
        if (bytesRead > 0) {
            decodedBuffer.append(buffer.data(), bytesRead);
            
            size_t decodeableSize = decodedBuffer.length() - (decodedBuffer.length() % 4);
            if (decodeableSize > 0) {
                std::string decoded;
                try {
                    decoded = decode(decodedBuffer.substr(0, decodeableSize));
                } catch (std::exception &e) {
                    throw e;
                }
                ofs.write(decoded.c_str(), decoded.length());
                
                decodedBuffer = decodedBuffer.substr(decodeableSize);
            }
            
            totalProcessed += bytesRead;
            
            if (callback) {
                callback(fileSize, totalProcessed);
            }
        }
    }
    
    if (!decodedBuffer.empty()) {
        std::string decoded;
        try {
            decoded = decode(decodedBuffer);
        } catch (std::exception &e) {
            throw e;
        }
        
        ofs.write(decoded.c_str(), decoded.length());
        
        if (callback) {
            callback(fileSize, fileSize);
        }
    }
    
    ifs.close();
    ofs.close();
    return true;
}

// 编码
std::string Base64Conver::encode(const unsigned char* data, size_t len) {
    
    std::string result;
    result.reserve(((len + 2) / 3) * 4);
    
    for (size_t i = 0; i < len; i += 3) {
        uint32_t byte1 = data[i];
        uint32_t byte2 = (i + 1 < len) ? data[i + 1] : 0;
        uint32_t byte3 = (i + 2 < len) ? data[i + 2] : 0;
        
        uint32_t triple = (byte1 << 16) | (byte2 << 8) | byte3;
        
        result.push_back(base64Chars[(triple >> 18) & 0x3F]);
        result.push_back(base64Chars[(triple >> 12) & 0x3F]);
        
        if (i + 1 < len) {
            result.push_back(base64Chars[(triple >> 6) & 0x3F]);
        } else {
            result.push_back('=');
        }
        
        if (i + 2 < len) {
            result.push_back(base64Chars[triple & 0x3F]);
        } else {
            result.push_back('=');
        }
    }
    
    return result;
}

// 解码
std::string Base64Conver::decode(const std::string& data) {
    std::vector<int> charMap(256, -1);
    for (int i = 0; i < 64; i++) {
        charMap[base64Chars[i]] = i;
    }
    
    std::string result;
    result.reserve((data.length() * 3) / 4);
    
    int val = 0;
    int valb = -8;
    
    for (unsigned char c : data) {
        if (c == '=') {
            break;
        }
        
        if (charMap[c] == -1) {
            throw "Invalid char";
        }
        
        val = (val << 6) | charMap[c];
        valb += 6;
        
        if (valb >= 0) {
            result.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    
    return result;
}