#ifndef XML_DOCUMENT_H
#define XML_DOCUMENT_H

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <ostream>
#include <istream>
#include <iostream>
#include <sstream>
#include <string>
#include <document.h>

using namespace std;
                                                                                
namespace XMLXPathReader {

  class XMLDocument {
    friend class BasicXPathReader;
  public:
                                                                                
    XMLDocument(const string& filename) {
      refcount = 0;
                                                                                
      // Call libxml2 to parse a file into a document.
      doc = xmlParseFile(filename.c_str());
                                                                                
      // Check the document is non null
      if ( doc == (xmlDocPtr)NULL ) {
        throw "Could not parse document";
      }
                                                                                
        // Stuff to do with dumping back node selections
      xmlIndentTreeOutput = 1;
      xmlKeepBlanksDefault(0);
    }
                                                                                
    XMLDocument(istream& is) {
      refcount = 0;
                                                                                
      string xml_document;
      xml_document = "";
                                                                                
      // Read the stream char by char
      int c = is.get();
      while( is.good() ) {
        // append to the internal document
        xml_document += c;
                                                                                
        // get next char
        c = is.get();
      }
                                                                                
      // Now parse the document from memory.
      doc = xmlParseMemory(xml_document.c_str(), xml_document.length());
                                                                                
      // Ensure success
      if ( doc == (xmlDocPtr)NULL ) {
        throw "Could not parse document";
      }
                                                                                
      xmlIndentTreeOutput = 1;
      xmlKeepBlanksDefault(0);
    }

  protected:
    void increaseRefcount() {
      refcount++;
#ifdef DEBUG_XML_REFCOUNT
      cout << "Refcount increased. It is now: " << refcount << endl;
#endif
    }
                                                                                
    void decreaseRefcount() {
      if ( refcount > 0 ) {
        refcount--;

#ifdef DEBUG_XML_REFCOUNT
        cout << "Refcount decreased.. It is now: " << refcount << endl;
#endif
      }
      else {
        ostringstream error_message;
        error_message << "Attempt to decrease refcount below 0";
        throw error_message.str();
      }
    }
                                                                                
    int getRefcount() {
      return refcount;
    }
                                                                                
    xmlDocPtr getDoc(void) {
      return doc;
    }
                                                                                
  private:
    int refcount;
    xmlDocPtr doc;
  };

};

#endif
