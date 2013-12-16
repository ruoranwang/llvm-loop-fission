#ifndef BP_H
#define BP_H
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "DG.h"
#include "machine.h"
#include <stdio.h>


namespace llvm{
typedef std::pair<Instruction*> inst_pair;
typedef std::vector<Instruction*> inst_vec;
typedef std::set<Instruction*> inst_set;
typedef std::set<std::pair<Instruction*> > inst_pair_set;
typedef std::map<Instruction*, bool> inst_visit;
typedef std::map<Instruction*, std::set<Instruction*> > inst_map_set;
typedef std::vector<std::vector<Instruction*> > inst_vec_vec;
typedef std::map<Loop*, std::vector<std::vector<Instruction*> > > loop_sccs;

  class AliasAnalysis;
  class DependenceAnalysis;
  class Value;
  class raw_ostream;

  class BP : public FunctionPass{
    public:
    AliasAnalysis *AA;
    Function *F;
    DependenceAnalysis *DA;
    LoopInfo *LI;
    DG *depmap;
  
//////functions/////
    inst_vec dfs(Instruction *start_inst, inst_map_set dg_of_loop, inst_set all_insts, inst_visit *visited);
    inst_vec_vec build_partition(Loop *CurL, inst_map_set CurInstMapSet); // build prdg (register deps)
    inst_map_set dual_dg_map(inst_map_set dg_inst_map); // helper function for build prdg
    inst_vec_vec build_scc(Loop *CurL, inst_pair_set dg_mem_map, inst_vec_vec prdg); // build scc (mem deps) based on prdg
    inst_pair_set find_dual(inst_map_set dg_mem_map);

// for heuristics //
    inst_vec_vec check_partition(inst_vec_vec old_scc, Loop* L);
    double cache_score (int * sizes);
    double parallel_score ();
    double add_inst_score (int cuts, int loop_header_cnt);
    int NumHeaderInst(Loop *L);

    std::map<Loop*, int> NumOfPartitions;
    loop_sccs	Partitions; 

    static char ID; 
    BP() : FunctionPass(ID) {};
    bool runOnFunction(Function &F);
    void getAnalysisUsage(AnalysisUsage &) const;
    void OutputBP(Loop *L);
    void dumpBP(Loop *L);

};

}//end llvm namespace
#endif
