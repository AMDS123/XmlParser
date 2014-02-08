//
//  xml.h
//  Version 0.1
//  Created by AMDS on 2010-1-8.
//

// This code is distributed under the terms and conditions of the MIT license.

// Copyright (c) 2010 AMDS
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef XMLNODE_H
#define XMLNODE_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
using namespace std;

class XmlNode
{
friend class Xml;
public:
	XmlNode(string lable,XmlNode* parent);
	XmlNode(string lable,XmlNode* parent,bool issingle);
	XmlNode(string lable,string content,XmlNode* parent);
	~XmlNode();
	void show(string flags,string flag);
	void setContent(string _content);
	string getContent();
	void save(ofstream &of,string flags);
	XmlNode& operator[](int index);//重载[]
	XmlNode& child(string _label);
	void deleteChild(string _label);
	void addChild(string _label);//添加孩子结点
	void addChild(string _label,string _content);//添加孩子结点
	void parseLabel();//解析标签
	void ltrim(string& str);//去除左面空格
	void rtrim(string& str);//去除右面空格
	void getValue(string &str,string &value);//提取值
	void setAttribute(string _attribute,string _value);
private:
	vector<XmlNode*> childs;
	vector<string> atrrindex;
	map<string,string> attributes;
	XmlNode *parent;	
	string lable;
	string content;	
	bool issingle;
};
typedef vector<XmlNode*>::iterator XmlIt;
typedef vector<XmlNode*>::size_type XmlST;
#endif