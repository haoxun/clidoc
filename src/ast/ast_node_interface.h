#ifndef SRC_AST_AST_NODE_INTERFACE_H_
#define SRC_AST_AST_NODE_INTERFACE_H_

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace clidoc {

// Defines terminal types.
enum class TerminalType {
  K_OPTIONS,
  K_DOUBLE_HYPHEN,

  POSIX_OPTION,
  GROUPED_OPTIONS,
  GNU_OPTION,
  ARGUMENT,
  DEFAULT_VALUE,
  COMMAND,

  GENERAL_ELEMENT,

  // for terminals that would not be instaniation.
  OTHER,
};

// Defines non-terminal types.
enum class NonTerminalType {
  // logical.
  LOGIC_AND,
  LOGIC_XOR,
  LOGIC_OPTIONAL,
  LOGIC_ONEORMORE,
  LOGIC_EMPTY,
  // start node.
  DOC,
};

const std::map<TerminalType, std::string> kTermianlClassName = {
  {TerminalType::K_OPTIONS,       "KOptions"},
  {TerminalType::K_DOUBLE_HYPHEN, "KDoubleHyphen"},

  {TerminalType::POSIX_OPTION,    "PosixOption"},
  {TerminalType::GROUPED_OPTIONS, "GroupedOptions"},
  {TerminalType::GNU_OPTION,      "GnuOption"},
  {TerminalType::ARGUMENT,        "Argument"},
  {TerminalType::COMMAND,         "Command"},

  {TerminalType::GENERAL_ELEMENT, "GeneralElement"},
};

const std::map<NonTerminalType, std::string> kNonTermianlClassName = {
  {NonTerminalType::DOC,             "Doc"},
  {NonTerminalType::LOGIC_AND,       "LogicAnd"},
  {NonTerminalType::LOGIC_XOR,       "LogicXor"},
  {NonTerminalType::LOGIC_OPTIONAL,  "LogicOptional"},
  {NonTerminalType::LOGIC_ONEORMORE, "LogicOneOrMore"},
};

// forward declaration for `SharedPtrNode`...
class NodeInterface;
// forward declaration for `NodeConnection::ConnectParent`.
template <NonTerminalType T>
class NonTerminal;

using SharedPtrNode = std::shared_ptr<NodeInterface>;
using WeakPtrNode = std::weak_ptr<NodeInterface>;
using VecSharedPtrNode = std::vector<SharedPtrNode>;

// Record the binding of parent and child.
struct NodeConnection {
  template <NonTerminalType T>
  void ConnectParent(NonTerminal<T> *parent_ptr) {
    this_child_ptr_ = &parent_ptr->children_.back();
    children_of_parent_ptr_ = &parent_ptr->children_;
  }

  SharedPtrNode *this_child_ptr_ = nullptr;
  VecSharedPtrNode *children_of_parent_ptr_ = nullptr;
};

struct NodeVistorInterface;

// Interface for symbols in parsing tree.
class NodeInterface {
 public:
  virtual ~NodeInterface() = default;

  // inline member helps generating indented prefix.
  std::string GetIndent(const int &indent) const;
  // get the string identify CURRENT node.
  virtual std::string GetID() = 0;
  // encode the tree structure rooted by current node as string.
  virtual std::string ToString() = 0;
  // indented version of ToString().
  virtual std::string ToString(const int &indent) = 0;
  // Apply vistor design pattern!
  virtual void Accept(NodeVistorInterface *vistor_ptr) = 0;
  // Connection of nodes in AST.
  NodeConnection node_connection;
};


// Basic element to store data.
class Token {
 public:
  // constructors.
  Token() = default;
  explicit Token(TerminalType type);
  Token(TerminalType type, const std::string &value);

  // support std::map.
  bool operator<(const Token &other) const;
  // support equality test.
  bool operator==(const Token &other) const;
  bool operator!=(const Token &other) const;

  bool IsEmpty() const;

  // accessors.
  bool has_value() const;
  TerminalType type() const;
  std::string value() const;
  // mutator.
  void set_value(const std::string &value);

 private:
  bool has_value_ = false;
  TerminalType type_ = TerminalType::OTHER;
  std::string value_;
};

}  // namespace clidoc

namespace clidoc {

// This member function must be marked inline, otherwise a linkage error would
// be raised.
inline std::string NodeInterface::GetIndent(const int &indent) const {
  std::string indent_element = "  ";
  std::ostringstream strm;
  for (int repeat_times = 0; repeat_times < indent; ++repeat_times) {
    strm << indent_element;
  }
  return strm.str();
}

inline Token::Token(TerminalType type)
    : has_value_(false), type_(type) { /* empty */ }

inline Token::Token(TerminalType type, const std::string &value)
    : has_value_(true), type_(type), value_(value) { /* empty */ }

// support std::map.
inline bool Token::operator<(const Token &other) const {
  return value_ < other.value_;
}

// support equality test.
inline bool Token::operator==(const Token &other) const {
  return has_value_ == other.has_value()
         && type_ == other.type()
         && value_ == other.value();
}

inline bool Token::operator!=(const Token &other) const {
  return !(*this== other);
}

inline bool Token::IsEmpty() const {
  // is TerminalType::OTHER or has empty value.
  return type_ == TerminalType::OTHER
         || (has_value_ && value_.empty());
}

// accessors.
inline bool Token::has_value() const {
  return has_value_;
}

inline TerminalType Token::type() const {
  return type_;
}

inline std::string Token::value() const {
  return value_;
}

// mutator.
inline void Token::set_value(const std::string &value) {
  value_ = value;
}

}  // namespace clidoc

#endif  // SRC_AST_AST_NODE_INTERFACE_H_
