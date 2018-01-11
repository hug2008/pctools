#include <iostream> 
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_iterators.hpp"

static const char *XML_NAME = "Rapi.xml";

using namespace rapidxml;

void rapiXmlCreat(const char *FileName_p)
{
	xml_document<> doc;  
	xml_node<>* rot = doc.allocate_node(node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
	doc.append_node(rot);
	xml_node<>* node =   doc.allocate_node(node_element,"config","information");  
	xml_node<>* color =   doc.allocate_node(node_element,"color",NULL);  
	doc.append_node(node);
	node->append_node(color);
	color->append_node(doc.allocate_node(node_element,"red","0.1"));
	color->append_node(doc.allocate_node(node_element,"green","0.1"));
	color->append_node(doc.allocate_node(node_element,"blue","0.1"));
	color->append_node(doc.allocate_node(node_element,"alpha","1.0"));

	xml_node<>* size =   doc.allocate_node(node_element,"size",NULL);
	size->append_node(doc.allocate_node(node_element,"x","640"));
	size->append_node(doc.allocate_node(node_element,"y","480"));
	node->append_node(size);

	xml_node<>* mode = doc.allocate_node(node_element,"mode","screen mode");
	mode->append_attribute(doc.allocate_attribute("fullscreen","false"));
	node->append_node(mode);

#if 0
	std::string text;  
	print(std::back_inserter(text), doc, 0);  

	std::cout<<text<<std::endl;
#endif

	std::ofstream out(FileName_p);
	out << doc;
}

void rapidxmlLoad(const char *FileName_p)
{
	file<> fdoc(FileName_p);

	std::cout<<fdoc.data()<<std::endl; 

	xml_document<>   doc;

	std::string szDeclaration = doc.parse<0>(fdoc.data());


	std::cout<<doc.name()<<std::endl;

	std::cout<<szDeclaration<<std::endl;

	//! 获取根节点
	xml_node<>* root = doc.first_node();
	std::cout<<root->name()<<std::endl;

	//! 获取根节点第一个节点
	xml_node<>* node1 = root->first_node();
	std::cout<<node1->name()<<std::endl;

	xml_node<>* node11 = node1->first_node();
	std::cout<<node11->name()<<std::endl;
	std::cout<<node11->value()<<std::endl;

	//! 修改之后再次保存
	xml_node<>* size = root->first_node("size");
	size->append_node(doc.allocate_node(node_element,"w","0"));
	size->append_node(doc.allocate_node(node_element,"h","0"));

	std::string text;  
	rapidxml::print(std::back_inserter(text),doc,0);  

	std::cout<<text<<std::endl;

	std::ofstream out("config.xml");
	out << doc;
}

int main(int argc, char *argv[], char *env[])
{
	system("echo rapiXmlCreat().....\n");
	
	rapiXmlCreat(XML_NAME);
	
	system("echo rapidxmlLoad().....\n");

	rapidxmlLoad(XML_NAME);

	system("PAUSE");

	return EXIT_SUCCESS;
}