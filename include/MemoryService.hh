#ifndef MemoryService_H
#define MemoryService_H 1
#ifdef MEMCHECK

#include "globals.hh"

class G4Run;
class G4Event;

///proc/[pid]/statm: summary of memory usages in pages.
struct proc_pid_statm {
  long vsize; // %lu
  long rss;   // %ld
  long share; // %ld
  long text;  // %ld
  long lib;   // %ld
  long data;  // %ld
  long dt;    // %ld
};

class MemoryService
{
public:

  MemoryService ();  
  ~MemoryService() {}

  void Start();
  void Stop();
  void Update();
  void Print(const G4Run* run);
  void Print(const G4Event* evt);

  G4double GetVsize() { return fVsize; }
  G4double GetRss() { return fRss; }
  G4double GetShare() { return fShare; }

private:

  G4double fVsize;
  G4double fRss;
  G4double fShare;
  G4double fPageSize;

  G4int fFile;
  mutable char fBuffer[256];

};

//this helper class is an excerpt of ProcInfoFetcher of CMSSW

#include <boost/lexical_cast.hpp>
#include <cassert>

class StreamBuffer {

public:
  explicit StreamBuffer(char* buffer) : 
    buffer_(buffer),
    save_(0),
    delims_(" \t\n\f\v\r") {}

  friend StreamBuffer& operator>>(StreamBuffer& sBuffer, long& oValue) {
    oValue = sBuffer.getLong();
    return sBuffer;
  }

private:
  long getLong() {
    const char* t = getItem();
    return boost::lexical_cast<long>(t);
  } 
  char* getItem() {
    char* item = strtok_r(buffer_, delims_, &save_); 
    assert(item);
    buffer_ = 0; // Null for subsequent strtok_r calls.
    return item;
  }

private:
  char* buffer_;
  char* save_;
  char const* const delims_; 
};
#endif
#endif
