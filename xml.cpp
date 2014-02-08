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

#include "xml.h"
#include "XmlNode.h"
#include <fstream>
#include <limits>
#include <sys/timeb.h>

Xml::Xml()
{
	head = new XmlNode("",NULL);
}

Xml::Xml(char *path)
{
	head = new XmlNode("",NULL);
	if(!parse(path)) 
	{
		delete head;
		head = NULL;
	}
}

Xml::~Xml()
{
	delete head;
	head = NULL;
}

bool Xml::isgraph(char ch)
{
	if (ch!=0&&ch!=9&&ch!=10&&ch!=' ')
		return true;
	return false;
}

bool Xml::parseLabel(string &aLabel,string &labeltext)
{
	size_t len = aLabel.length();
	size_t pos = min(aLabel.find(' '),aLabel.find('\n'));
	if ( pos == -1 || pos > len)
	{
		pos = aLabel.find('>');
	}
	if (aLabel[1] == '/')
	{
		labeltext = aLabel.substr(2,pos-2);
	}
	else if (aLabel[len-2]=='/')
	{
		labeltext = aLabel.substr(1,pos-1);
		size_t mlen = labeltext.length();
		if (labeltext[mlen-1]=='/') 
		{
			labeltext = labeltext.substr(0,mlen-1);
		}
	}
	else
	{
		labeltext = aLabel.substr(1,pos-1);
	}
	
	return true;
}

bool Xml::parse(char* path)
{
	timeb start,end;
	ftime(&start);
	ifstream xmlfile(path,ios::in);
	if (!xmlfile)
	{
		cout<<¡°Open file failed!¡±<<endl;
		return false;
	}
	char ch;
	
	string label;
	string labeltext;
	string content;
	bool ismatch = false;
	bool iscontent = false;

	string title;

	XmlNode * cur = head;

	while (xmlfile.eof()!=ios::eofbit)
	{
		xmlfile.get(ch);
	
		if (ch=='<') 
		{
			ismatch = true;
			if (iscontent&&content.length())
			{
				cur->setContent(content);
				content.clear();
			}
			iscontent = false;
		}
		if (ismatch)
		{
			label += ch;
		}
		if (iscontent&&isgraph(ch))
		{
			content += ch;
		}
		if (label.substr(0,5)=="<?xml"&&label.substr(label.length()-2,2)=="?>")
		{
			m_strMsg = label;
			label.clear();
			ismatch = false;
		}
		else if (label.substr(0,4)=="<!--"&&label.substr(label.length()-3,3)=="-->")
		{
			label.clear();
			ismatch = false;
		}
		else if(label[0]=='<'&&label[label.length()-1]=='>')
		{
			if (label[1]=='/')
			{
				parseLabel(label,labeltext);

				if (m_vtag.back()!=labeltext)
				{
					cout<<¡°match¡±<<labeltext<<¡°failed!¡±<<endl;
					xmlfile.close();
					return false;
				}
				m_vtag.pop_back();
				cur = cur->parent;
			}
			else if (label[label.length()-2]=='/')
			{
				parseLabel(label,labeltext);
				label = label.substr(1,label.length()-3);
				addChild(label,cur,true);
				counts[labeltext]++;
			}
			else
			{
				parseLabel(label,labeltext);
				label = label.substr(1,label.length()-2);
				addChild(label,cur);
				cur = *(cur->childs.end()-1);
				iscontent = true;
				
				m_vtag.push_back(labeltext);
				counts[labeltext]++;
			}
			
			label.clear();
			ismatch = false;
			
		}

		
	}
	xmlfile.close();
	ftime(&end);
	cout<<¡°parse consume ¡±<<((double(end.time)-double(start.time))*1000.0+double(end.millitm)-double(start.millitm))<<"millisecond"<<endl;
	return true;
}

void Xml::addChild(string label,XmlNode* parent)
{
	parent->childs.push_back(new XmlNode(label,parent));
}
void Xml::addChild(string label,XmlNode* parent,bool issingle)
{
	parent->childs.push_back(new XmlNode(label,parent,issingle));
}


XmlNode& Xml::child(string label)
{
	return head->child(label);
}

void Xml::show(string flag)
{
	if (head==NULL) return;
	string flags;
	cout<<m_strMsg<<endl;
	head->show(flags,flag);
}

bool Xml::save(char *path)
{
	ofstream outfile(path,ios::out);
	if (!outfile) return false;
	outfile<<m_strMsg<<endl;
	string flags;
	head->save(outfile,flags);
	outfile.close();
	return true;
}

void Xml::menu()
{
	printf("\n\n\n\n");
	printf("\t\t**********************************\n");
	printf("\t\t*                                *\n");
	printf("\t\t*   Welcome to use parser V0.1   *\n");
	printf("\t\t*                                *\n");
	printf("\t\t**********************************\n");
	printf("\n\t\t");
	system("pause");
	system("cls");

	bool isopen = false;
	int choice;
	char path[100];
	string str;
	string str1;
	XmlNode *node = NULL;
	
	string label;
	string content;

	while (1)
	{
		cout<<¡°0.Open File¡±<<endl;
		cout<<¡°1.Show File¡±<<endl;
		cout<<¡°2.Show Node¡°<<endl;
		cout<<¡°3.Add Node¡°<<endl;
		cout<<¡°4.Edit Node¡°<<endl;
		cout<<¡°5.Delete Node¡°<<endl;
		cout<<¡°6.Edit Attribute¡±<<endl;
		cout<<¡°7.Node Statistics"<<endl;
		cout<<¡°8.Save File¡±<<endl;
		cout<<¡°9.Exit¡±<<endl;


		cout<<¡°Please enter your choice£º¡±<<endl;
		cin>>choice;
		cin.clear();
		cin.ignore( numeric_limits<streamsize>::max(), '\n' );

		switch (choice)
		{
		case 0:
			delete head;
			head = new XmlNode("",NULL);
			cout<<¡°Please enter the file path¡±<<endl;
			cin>>path;
			cout<<¡°Parsingÿ¡±<<endl;
			if (parse(path))
			{
				cout<<¡°Parse Complete¡±<<endl;
				isopen = true;
			}
			else
			{
				cout<<¡°Parse Failed!¡±<<endl;
				isopen = false;
			}
			
			
			break;
		case 1:
			if (!isopen)
			{
				cout<<¡°Please Open File First!¡±<<endl;
				break;
			}
			cout<<¡°Please enter placeholder:¡±<<endl;
			getline(cin,str);
			show(str);
			break;
		case 2:
			if (!isopen)
			{
				cout<<"Please Open File First!"<<endl;
				break;
			}
			try
			{
				locate(node);
				cout<<node->content<<endl;	
			}
			catch (string e)
			{
				cout<<e<<endl;
			}
			
					
			break;
		case 3:
			if (!isopen)
			{
				cout<<"Please Open File First!"<<endl;
				break;
			}
			
			try
			{
				locate(node);
				cout<<¡°Please enter label¡±<<endl;
				cin>>label;
				cin.clear();
				cin.ignore( numeric_limits<streamsize>::max(), '\n' );
				cout<<"Please enter content¡±<<endl;
				getline(cin,content);
				node->addChild(label,content);
			}
			catch (string e)
			{
				cout<<e<<endl;
			}
			
			break;
		case 4:
			if (!isopen)
			{
				cout<<"Please Open File First!"<<endl;
				break;
			}
			try
			{
				locate(node);
				cout<<"Please enter content"<<endl;
				cin>>content;
				node->setContent(content);
			}
			catch (string e)
			{
				cout<<e<<endl;
			}
			
			break;
		case 5:
			if (!isopen)
			{
				cout<<"Please Open File First!"<<endl;
				break;
			}
			try
			{
				locate(node);
				node->parent->deleteChild(node->lable);
				cout<<¡°Delete Succeed!¡±<<endl;
			}
			catch (string e)
			{
				cout<<e<<endl;
				cout<<"Delete Failed!¡±<<endl;
			}			
			break;
		case 6:
			if (!isopen)
			{
				cout<<"Please Open File First!"<<endl;
				break;
			}
			try
			{
				locate(node);
				cout<<"Please enter attribute name¡±<<endl;
				cin>>str;
				cout<<"Please enter attribute value¡±<<endl;
				cin>>str1;
				node->setAttribute(str,str1);
				cout<<¡°Edit Succeed!¡±<<endl;
			}
			catch (string e)
			{
				cout<<e<<endl;
			}
			break;
		case 7:
			if (!isopen)
			{
				cout<<"Please Open File First!"<<endl;
				break;
			}
			cout<<¡°Please enter node¡±<<endl;
			cin>>str;
			if (counts[str]==0)
			{
				cout<<¡°no this node¡±<<endl;
			}
			else
			{
				cout<<¡°node¡±<<str<<¡°count£º¡±<<counts[str]<<endl;
			}
			
			break;
		case 8:
			if (!isopen)
			{
				printf("Please Open File First!\n");
				break;
			}
			cout<<"Please enter the file path¡°<<endl;
			cin>>path;
			save(path);
			break;
		case 9:
			cout<<¡°Welcome to use this system.¡±<<endl;
			return;
			break;
		default:
			cout<<¡°Enter Error!¡±<<endl;
			break;
		}
		system("pause");
		system("cls");		
	}
}

//Split
void Xml::strexplod(string str,vector<string> &items)
{
	size_t pos;
	while ((pos=str.find("->"))!=-1)
	{
		items.push_back(str.substr(0,pos));
		if (pos >= str.size()-1) break;
		str = str.substr(pos+2);
	}
	items.push_back(str);
}

// get [] value
void Xml::strindex(string &str,int &index)
{
	size_t pos1 = str.find("[");
	size_t pos2 = str.find("]");
	string tmp;
	if (pos2 - pos1 > 0)
	{
		tmp = str.substr(pos1+1,pos2-pos1-1);
		index = atoi(&tmp[0]);
		str = str.substr(0,pos1);
	}
	else
	{
		index = -1;
	}
}

//locate node
void Xml::locate(XmlNode * &node)
{
	vector<string> items;
	string str;
	cout<<¡°enter node (split by ->)¡±<<endl;
	fflush(stdin);
	getline(cin,str);
	fflush(stdin);
	strexplod(str,items);
	int index = 0;

	node = &head->child(items[0]);
	for(int i=1;i<items.size();i++)
	{
		strindex(items[i],index);
		if (index == -1)
		{
			node = &node->child(items[i]);
		}
		else
		{
			node = &(node->child(items[i])[index]);
		}
	}
}