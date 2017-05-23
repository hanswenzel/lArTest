#include "MemoryService.hh"

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

#include <string>
#include <iostream>

#include <fcntl.h>
#include <unistd.h>

MemoryService::MemoryService() :
  fVsize(0.), fRss(0.), fShare(0.)
{
  fPageSize = sysconf(_SC_PAGESIZE); 
}  

void MemoryService::Start() 
{
  std::ostringstream ost;
  ost << "/proc/" << getpid() << "/statm";
  
  if((fFile = open(ost.str().c_str(), O_RDONLY)) < 0) {
    G4Exception("MemoryService::Start()", "FAIL",
		FatalException, "Fail to open /prod/[pid]/statm");
  }
}

void MemoryService::Stop()
{
  close(fFile);
}  

void MemoryService::Update() 
{
  proc_pid_statm pstatm;
  G4int cnt;
  lseek(fFile, 0, SEEK_SET);
       
  if((cnt = read(fFile, fBuffer, sizeof(fBuffer) - 1)) < 0) {
    perror("Warning: Fail to read of /proc/[pid]/statm file");
  }
 
  if(cnt > 0) {
    fBuffer[cnt] = '\0';

    auto vsize = 0;
    auto rss   = 0;
    auto share = 0;

    try {
      StreamBuffer fetcher(fBuffer);
      fetcher >> pstatm.vsize
	      >> pstatm.rss
	      >> pstatm.share
	      >> pstatm.text
	      >> pstatm.lib
	      >> pstatm.data
	      >> pstatm.dt;

      vsize = pstatm.vsize;
      rss   = pstatm.rss;
      share = pstatm.share;

    } catch (boost::bad_lexical_cast& iE) {
      std::cout <<"Parsing of prof/[pid]/statm file failed:" << iE.what() << 
      std::endl;
    }

    //convert to MB
    const G4double inMegabyte = fPageSize/(1024.0*1024.0);
    fVsize = vsize*inMegabyte;
    fRss   =   rss*inMegabyte;
    fShare = share*inMegabyte;
  }
}

void MemoryService::Print(const G4Event* evt)
{
  G4int precision_t = G4cout.precision(3);
  std::ios::fmtflags flags_t = G4cout.flags();
  G4cout.setf(std::ios::fixed,std::ios::floatfield); 

  G4cout << "MemoryEvt> "<< evt->GetEventID()+1 << " "
	 << G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID()+1 << " "
	 << GetVsize() << " " 
	 << GetRss() << " " 
	 << GetShare() << " " << G4endl;
  G4cout.setf(flags_t);
  G4cout.precision(precision_t);
}

void MemoryService::Print(const G4Run* )
{
  G4int precision_t = G4cout.precision(3);
  std::ios::fmtflags flags_t = G4cout.flags();
  G4cout.setf(std::ios::fixed,std::ios::floatfield); 

  G4cout << "MemoryReport> Memory [VSIZE,RSS,SHR] report complete in "
         << GetVsize() << " " 
         << GetRss() << " " 
         << GetShare() << G4endl;
  G4cout.setf(flags_t);
  G4cout.precision(precision_t);
}
