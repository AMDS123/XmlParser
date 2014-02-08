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

#ifndef XML_H
#define XML_H

#include <iostream>
#include <string>
#include <vector>
#include "XmlNode.h"
using namespace std;

class Xml
{
public:
	Xml();
	Xml(char *path);
	~Xml();
	void addMsg(string _msg);
	void addChild(string label,XmlNode* parent);//添加子节点
	void Xml::addChild(string label,XmlNode* parent,bool issingle);
	XmlNode& child(string label);//得到子节点
	bool parse(char* path);	//文件解析
	void show(string flag);//节点遍历
	bool isgraph(char ch);
	bool save(char *path);//文件保存
	bool parseLabel(string &aLabel,string &labeltext);//解析标签
	void menu();//菜单
	void strexplod(string str,vector<string> &items);
	void strindex(string &str,int &index);
	void locate(XmlNode * &node);
	map<string,int> counts;
private:
	vector<string> m_vtag;
	XmlNode* head;
	string m_strMsg;
};

#endif