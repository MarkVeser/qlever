// Copyright 2015, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author:
//   2015-2017 Björn Buchhold (buchhold@informatik.uni-freiburg.de)
//   2018-     Johannes Kalmbach (kalmbach@informatik.uni-freiburg.de)
//
// Copyright 2025, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)

#include "engine/Join.h"
#include "engine/JoinImpl.h"

#include <sstream>
#include <vector>

#include "JoinWithIndexScanHelpers.h"
#include "backports/functional.h"
#include "backports/type_traits.h"
#include "engine/IndexScan.h"
#include "engine/JoinHelpers.h"
#include "engine/Service.h"
#include "global/Constants.h"
#include "global/Id.h"
#include "global/RuntimeParameters.h"
#include "util/Algorithm.h"
#include "util/Exception.h"
#include "util/Generators.h"
#include "util/Iterators.h"

using namespace qlever::joinHelpers;
using namespace qlever::joinWithIndexScanHelpers;
using std::endl;
using std::string;


// _____________________________________________________________________________
Join::Join(QueryExecutionContext* qec, std::shared_ptr<QueryExecutionTree> t1,
           std::shared_ptr<QueryExecutionTree> t2, ColumnIndex t1JoinCol,
           ColumnIndex t2JoinCol, bool keepJoinColumn,
           bool allowSwappingChildrenOnlyForTesting)
    : Operation(qec), 
    _impl{std::make_unique<JoinImpl>(qec, std::move(t1), std::move(t2), t1JoinCol,
                                t2JoinCol, keepJoinColumn,
                                allowSwappingChildrenOnlyForTesting)} {}

// _____________________________________________________________________________
Join::Join(QueryExecutionContext* qec, std::unique_ptr<JoinImpl>&& impl)
    : Operation(qec), _impl{std::move(impl)} {}

// _____________________________________________________________________________
Join::~Join() = default;
Join::Join(Join&&) = default;
Join& Join::operator=(Join&&) = default;

// _____________________________________________________________________________
string Join::getDescriptor() const { return _impl->getDescriptor(); }

// _____________________________________________________________________________
string Join::getCacheKeyImpl() const {
  return _impl->getCacheKeyImpl();
}

// _____________________________________________________________________________
size_t Join::getResultWidth() const { return _impl->getResultWidth(); }

// _____________________________________________________________________________
std::vector<ColumnIndex> Join::resultSortedOn() const {
  return _impl->resultSortedOn();
}

// _____________________________________________________________________________
Result Join::computeResult(bool requestLaziness) {
  return _impl->computeResult(requestLaziness);
}

// _____________________________________________________________________________
bool Join::knownEmptyResult() { return _impl->knownEmptyResult(); }

// _____________________________________________________________________________
float Join::getMultiplicity(size_t col) {
  return _impl->getMultiplicity(col);
}

// _____________________________________________________________________________
size_t Join::getCostEstimate() {
  return _impl->getCostEstimate();
}

// _____________________________________________________________________________
std::vector<QueryExecutionTree*> Join::getChildren() {
  return _impl->getChildren();
}

// _____________________________________________________________________________
bool Join::columnOriginatesFromGraphOrUndef(const Variable& variable) const {
  return _impl->columnOriginatesFromGraphOrUndef(variable);
}

// _____________________________________________________________________________
std::unique_ptr<Operation> Join::cloneImpl() const {
  auto ptr = std::unique_ptr<JoinImpl>(
    static_cast<JoinImpl*>(_impl->cloneImpl().release()));
  return std::make_unique<Join>(_impl->getExecutionContext(), std::move(ptr));
}

// _____________________________________________________________________________
uint64_t Join::getSizeEstimateBeforeLimit() {
  return _impl->getSizeEstimateBeforeLimit();
}

// _____________________________________________________________________________
VariableToColumnMap Join::computeVariableToColumnMap() const {
  return _impl->computeVariableToColumnMap();
}
