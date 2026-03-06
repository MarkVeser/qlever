// Copyright 2015, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author:
//   2015-2017 Björn Buchhold (buchhold@informatik.uni-freiburg.de)
//   2018-     Johannes Kalmbach (kalmbach@informatik.uni-freiburg.de)

#ifndef QLEVER_SRC_ENGINE_JOIN_H
#define QLEVER_SRC_ENGINE_JOIN_H

#include <memory>

#include "backports/concepts.h"
#include "engine/AddCombinedRowToTable.h"
#include "engine/IndexScan.h"
#include "engine/Operation.h"
#include "engine/QueryExecutionTree.h"
#include "util/JoinAlgorithms/JoinColumnMapping.h"
#include "util/TypeTraits.h"

class JoinImpl;
class Join : public Operation {
 public:
  // `allowSwappingChildrenOnlyForTesting` should only ever be changed by tests.
  Join(QueryExecutionContext* qec, std::shared_ptr<QueryExecutionTree> t1,
       std::shared_ptr<QueryExecutionTree> t2, ColumnIndex t1JoinCol,
       ColumnIndex t2JoinCol, bool keepJoinColumn = true,
       bool allowSwappingChildrenOnlyForTesting = true);

  // Internal constructor used for the implementation of `clone`.
  Join(QueryExecutionContext* qec, std::unique_ptr<JoinImpl>&& impl);
  
  ~Join() override;
  Join(Join&&);
  Join& operator=(Join&&);

  // Virtual functions inherited from the 'Operation' base class.
  std::string getDescriptor() const override;
  virtual std::string getCacheKeyImpl() const override;
  size_t getResultWidth() const override;
  std::vector<ColumnIndex> resultSortedOn() const override;
  bool knownEmptyResult() override;
  float getMultiplicity(size_t col) override;
  size_t getCostEstimate() override;
  std::vector<QueryExecutionTree*> getChildren() override;
  Result computeResult(bool requestLaziness) override;
  bool columnOriginatesFromGraphOrUndef(
      const Variable& variable) const override;
  std::unique_ptr<Operation> cloneImpl() const override;

 private:
  std::unique_ptr<JoinImpl> _impl;
  uint64_t getSizeEstimateBeforeLimit() override;
  VariableToColumnMap computeVariableToColumnMap() const override;
};

#endif  // QLEVER_SRC_ENGINE_JOIN_H
