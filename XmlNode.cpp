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

#include "XmlNode.h"

XmlNode::XmlNode(string _lable,XmlNode *_parent)
{
	lable = _lable;
	parent = _parent;
	content = "";
	issingle = false;
	parseLabel();
}

XmlNode::XmlNode(string _lable,XmlNode* _parent,bool _issingle)
{
	lable = _lable;
	parent = _parent;
	issingle = _issingle;
	parseLabel();
}

XmlNode::XmlNode(string _lable,string _content,XmlNode *_parent)
{
	lable = _lable;
	parent = _parent;
	content = _content;
	issingle = false;
	parseLabel();
}


XmlNode::~XmlNode()
{
	XmlIt it = childs.begin();
	for (;it<childs.end();it++)
	{
		delete (*it);
	}
	childs.clear();
}

//show xml
void XmlNode::show(string flags,string flag)
{
	vector<XmlNode*>::iterator it;
	for (it=childs.begin();it<childs.end();it++)
	{
		cout<<flags<<"<"<<(*it)->lable;
		for (int i=0;i<(*it)->atrrindex.size();i++)
		{
			cout <<" " << (*it)->atrrindex[i] << " = ";
			cout << (*it)->attributes[(*it)->atrrindex[i]];
		}
			
		if ((*it)->issingle)
		{
			cout<<"/>"<<endl;
		}
		else
		{
			cout<<">";
			if (!(*it)->content.length())
			{
				cout<<endl;
			}
			(*it)->show(flags+flag,flag);
			if ((*it)->content.length())
			{
				cout<<(*it)->content;
			}
			else
			{
				cout<<flags;
			}
			cout<<"</"<<(*it)->lable<<">"<<endl;
		}
		
	}
}

//edit xmlnode
void XmlNode::setContent(string _content)
{
	content = _content;
}

//save xml
void XmlNode::save(ofstream &of,string flags)
{
	vector<XmlNode*>::iterator it;
	for (it=childs.begin();it<childs.end();it++)
	{
		of<<flags<<"<"<<(*it)->lable;
		for (int i=0;i<(*it)->atrrindex.size();i++)
		{
			of <<" " << (*it)->atrrindex[i] << " = ";
			of << (*it)->attributes[(*it)->atrrindex[i]];
		}
		if ((*it)->issingle)
		{
			of<<"/>"<<endl;
		}
		else 
		{
			of<<">";	
			if (!(*it)->content.length())
			{
				of<<endl;
			}
			(*it)->save(of,flags+"	");
			if ((*it)->content.length())
			{
				of<<(*it)->content;
			}
			else
			{
				of<<flags;
			}
			of<<"</"<<(*it)->lable<<">"<<endl;
		}
		
	}
}

//reload operator[]
XmlNode& XmlNode::operator[](int index)
{
	XmlIt it = parent->childs.begin();
	int i = 0;
	for (;it<parent->childs.end();it++)
	{
		if ((*it)->lable==lable)
		{			
			if (i++==index) break;
		}
	}
	if (it == parent->childs.end())
	{
		string str = "Can not find this Node!";
		throw str;
	}
	return **it;
}

//get node content
string XmlNode::getContent()
{
	return content;
}

//get childnode
XmlNode& XmlNode::child(string _lable)
{
	XmlIt it = childs.begin();
	for (;it<childs.end();it++)
	{
		if ((*it)->lable==_lable)
		{
			return **it;
		}
	}
	string str = "Can not find this Node!";
	throw str;
	return XmlNode("",NULL);
}

void XmlNode::deleteChild(string _lable)
{
	XmlIt it = childs.begin();
	for (;it<childs.end();it++)
	{
		if ((*it)->lable==_lable)
		{
			childs.erase(it);
			break;
		}
	}
}

void XmlNode::addChild(string _label)
{ 
	childs.push_back(new XmlNode(_label,this));
}

void XmlNode::addChild(string _label,string _content)
{ 
	childs.push_back(new XmlNode(_label,_content,this));
}




void XmlNode::parseLabel()
{
	string tmp = lable;
	size_t pos = lable.find(' ');
	string matr;
	string mval;
	if (pos!=-1)
	{
		lable = lable.substr(0,pos);
		tmp = tmp.substr(pos+1);

		while ((pos=tmp.find('='))!=-1)
		{
			matr = tmp.substr(0,pos);
			rtrim(matr);
			tmp = tmp.substr(pos+1);
			ltrim(tmp);
			getValue(tmp,mval);
			ltrim(tmp);
			attributes[matr] = mval;
			atrrindex.push_back(matr);
		}
	}
	
}

void XmlNode::ltrim(string &str)
{
	size_t pos = 0;
	while (str[pos] == ' ') pos++;
	str = str.substr(pos);
}

void XmlNode::rtrim(string &str)
{
	size_t pos = str.size() - 1;
	while (str[pos] == ' ')
	{
		pos--;
	}
	str = str.substr(0,pos+1);
}

void XmlNode::getValue(string &str,string &mvalue)
{
	size_t pos1 = str.find(' ');
	size_t pos2 = str.find('=');
	size_t pos;
	pos = min(pos1,pos2);
	if (pos1 == -1)
	{
		pos = pos2;
	}
	if (pos2 == -1)
	{
		pos = pos1; 
	}
	mvalue = str.substr(0,pos);
	if (pos < str.size()-1)
	{
		str = str.substr(pos);
	}
}

void XmlNode::setAttribute(string _attribute,string _value)
{
	if (attributes[_attribute].length()==0)
	{
		string err = "No this Attribute";
		throw err;
	}
	attributes[_attribute] = _value;
}