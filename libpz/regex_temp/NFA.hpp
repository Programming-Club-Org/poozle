#ifndef PZ_REGEX_HPP
#define PZ_REGEX_HPP

#include <pz_cxx_std.hpp>
#include <pz_types.hpp>
#include <pz_error.hpp>

/** @brief namespace PzRegex */
namespace PzRegex {
enum class StateType;      // NFA state types
enum class AssertionType;  // Assertion types for regex anchors
struct CharClass;    // Character class representation
struct State;        // NFA state node
struct PtrList;      // Linked list for patching transitions
struct Frag;         // NFA fragment during construction
struct CaptureGroup; // Capture group information
class NFABuilder;    // NFA construction from postfix regex
class NFASimulator;  // NFA simulation engine
}; // namespace PzRegex

/**
 * @name State type enumeration
 * @brief Different types of states in the NFA.
 * @details
 * Each state type represents a different operation in the regex:
 * character matching, epsilon transitions, accepting states, etc.
 */
enum class PzRegex::StateType : st32 {
  STATE_CHAR = 0,      // Consume a character
  STATE_SPLIT,         // ε-transition (alternation)
  STATE_MATCH,         // Accepting state
  STATE_CHARCLASS,     // Character class [a-z]
  STATE_ASSERTION,     // ^, $, \b
  STATE_CAPTURE_START, // Start of capture group
  STATE_CAPTURE_END    // End of capture group
};

/**
 * @name Assertion type enumeration
 * @brief Types of zero-width assertions in regex patterns.
 * @details
 * Assertions match positions rather than characters.
 */
enum class PzRegex::AssertionType : st32 {
  ASSERT_NONE = 0,   // No assertion
  ASSERT_START_LINE, // ^ - Start of line
  ASSERT_END_LINE,   // $ - End of line
  ASSERT_WORD_BOUND  // \b - Word boundary
};

// Convenience type aliases
using PzStateType = PzRegex::StateType;
using PzAssertionType = PzRegex::AssertionType;

/**
 * @brief Character class for pattern matching
 */
namespace PzRegex {
struct CharClass {
  std::set<char> chars; // Characters in the class
  bool negated = false; // If true, matches characters NOT in set

  CharClass() = default;

  void addRange(char start, char end); // Add character range [start-end]
  void addChar(char c);                // Add single character
  bool matches(char c) const;          // Check if character matches class
};

/**
 * @brief NFA state node
 */
struct State {
  StateType type;                    // Type of state
  st32 c;                            // Character for STATE_CHAR
  State *out = nullptr;              // First transition (raw pointer)
  State *out1 = nullptr;             // Second transition (raw pointer for SPLIT)
  st32 lastlist = 0;                 // Bookkeeping for simulation

  // Extended features
  std::unique_ptr<CharClass> charClass = nullptr; // For STATE_CHARCLASS
  AssertionType assertion = ASSERT_NONE;          // For STATE_ASSERTION
  st32 captureIndex = -1;                         // For capture groups
  bool greedy = true; // Greedy vs non-greedy matching

  State(StateType t, st32 ch = 0, State *o = nullptr, State *o1 = nullptr);

  State(st32 t, State *o = nullptr, State *o1 = nullptr);
};

/**
 * @brief Linked list of state pointers for patching
 */
struct PtrList {
  State **p;                         // Pointer to a State raw pointer
  std::unique_ptr<PtrList> next;     // Next item in list

  PtrList(State **ptr, std::unique_ptr<PtrList> n = nullptr);
};

/**
 * @brief NFA fragment during construction
 */
struct Frag {
  std::unique_ptr<State> start;      // Start state of fragment (owned)
  std::unique_ptr<PtrList> out;      // Dangling output pointers

  Frag(std::unique_ptr<State> s = nullptr,
       std::unique_ptr<PtrList> o = nullptr);
};

/**
 * @brief Captured group information
 */
struct CaptureGroup {
  st32 start_pos; // Starting position
  st32 end_pos;   // Ending position
  std::string text; // Captured text
};

/**
 * @brief NFA builder from postfix regex
 */
class NFABuilder {
private:
  State *matchstate_; // The accepting state (non-owning)
  st32 next_capture_index_ = 0;       // Counter for capture groups

  static void patch(PtrList *l, State *s); // Patch dangling pointers
  static std::unique_ptr<PtrList>
  append(std::unique_ptr<PtrList> l1,
         std::unique_ptr<PtrList> l2); // Append PtrLists

public:
  NFABuilder();

  std::unique_ptr<State>
  build(const std::string &postfix); // Build NFA from postfix regex
  State *get_match_state() const;    // Get match state
  st32 get_capture_count() const;    // Get capture group count
};

/**
 * @brief NFA simulation engine for pattern matching
 */
class NFASimulator {
private:
  struct ListItem {
    State *state; // Non-owning pointer to state
    std::vector<CaptureGroup> caps;
  };

  struct List {
    std::vector<ListItem> items;
    void clear();
  };

  State *start_;                       // Start state (non-owning)
  State *matchstate_;                  // Match state (non-owning)
  st32 listid_ = 0;                    // Generation counter
  List l1_, l2_;                       // Current and next state lists
  st32 num_capture_groups_;            // Number of capture groups
  std::vector<CaptureGroup> captures_; // Storage for captures

  void addState(List *l, State *s, const std::string &input, st32 pos,
                std::vector<CaptureGroup> caps); // Add state to list
  bool checkAssertion(State *s, const std::string &input,
                      st32 pos); // Check assertion at position
  bool isWordBoundary(const std::string &input,
                      st32 pos); // Check word boundary
  List *startList(State *s, List *l, st32 pos); // Initialize state list
  void step(List *clist, const std::string &input, st32 pos,
            List *nlist); // Execute simulation step
  bool isMatch(List *l, const std::string &input,
               st32 pos); // Check for match state

public:
  NFASimulator(State *start, State *match, st32 numCaptures);

  bool match(const std::string &s);         // Match string against NFA
  std::string get_capture(st32 index) const; // Get captured text
};

} // namespace PzRegex

#endif // PZ_REGEX_HPP