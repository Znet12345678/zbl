
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __javax_swing_text_StyledDocument__
#define __javax_swing_text_StyledDocument__

#pragma interface

#include <java/lang/Object.h>
#include <gcj/array.h>

extern "Java"
{
  namespace java
  {
    namespace awt
    {
        class Color;
        class Font;
    }
  }
  namespace javax
  {
    namespace swing
    {
      namespace event
      {
          class DocumentListener;
          class UndoableEditListener;
      }
      namespace text
      {
          class AttributeSet;
          class Element;
          class Position;
          class Segment;
          class Style;
          class StyledDocument;
      }
    }
  }
}

class javax::swing::text::StyledDocument : public ::java::lang::Object
{

public:
  virtual ::javax::swing::text::Style * addStyle(::java::lang::String *, ::javax::swing::text::Style *) = 0;
  virtual void removeStyle(::java::lang::String *) = 0;
  virtual ::javax::swing::text::Style * getStyle(::java::lang::String *) = 0;
  virtual void setCharacterAttributes(jint, jint, ::javax::swing::text::AttributeSet *, jboolean) = 0;
  virtual void setParagraphAttributes(jint, jint, ::javax::swing::text::AttributeSet *, jboolean) = 0;
  virtual ::javax::swing::text::Style * getLogicalStyle(jint) = 0;
  virtual void setLogicalStyle(jint, ::javax::swing::text::Style *) = 0;
  virtual ::javax::swing::text::Element * getParagraphElement(jint) = 0;
  virtual ::javax::swing::text::Element * getCharacterElement(jint) = 0;
  virtual ::java::awt::Color * getForeground(::javax::swing::text::AttributeSet *) = 0;
  virtual ::java::awt::Color * getBackground(::javax::swing::text::AttributeSet *) = 0;
  virtual ::java::awt::Font * getFont(::javax::swing::text::AttributeSet *) = 0;
  virtual void addDocumentListener(::javax::swing::event::DocumentListener *) = 0;
  virtual void addUndoableEditListener(::javax::swing::event::UndoableEditListener *) = 0;
  virtual ::javax::swing::text::Position * createPosition(jint) = 0;
  virtual ::javax::swing::text::Element * getDefaultRootElement() = 0;
  virtual ::javax::swing::text::Position * getEndPosition() = 0;
  virtual jint getLength() = 0;
  virtual ::java::lang::Object * getProperty(::java::lang::Object *) = 0;
  virtual JArray< ::javax::swing::text::Element * > * getRootElements() = 0;
  virtual ::javax::swing::text::Position * getStartPosition() = 0;
  virtual ::java::lang::String * getText(jint, jint) = 0;
  virtual void getText(jint, jint, ::javax::swing::text::Segment *) = 0;
  virtual void insertString(jint, ::java::lang::String *, ::javax::swing::text::AttributeSet *) = 0;
  virtual void putProperty(::java::lang::Object *, ::java::lang::Object *) = 0;
  virtual void remove(jint, jint) = 0;
  virtual void removeDocumentListener(::javax::swing::event::DocumentListener *) = 0;
  virtual void removeUndoableEditListener(::javax::swing::event::UndoableEditListener *) = 0;
  virtual void render(::java::lang::Runnable *) = 0;
  static ::java::lang::Class class$;
} __attribute__ ((java_interface));

#endif // __javax_swing_text_StyledDocument__