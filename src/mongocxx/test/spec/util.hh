// Copyright 2018-present MongoDB Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <functional>

#include <bsoncxx/document/view.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/test/spec/operation.hh>

#include <mongocxx/config/private/prelude.hh>

namespace mongocxx {
MONGOCXX_INLINE_NAMESPACE_BEGIN
namespace spec {

using namespace bsoncxx;
using namespace mongocxx;

///
/// Returns true if this test should be skipped for any reason (for
/// example, if a skipReason is defined, or if the given topology is not
/// supported for this test.
///
bool should_skip_spec_test(const client& client, document::view test);

///
/// Configures the fail point described by test["failPoint"].
///
void configure_fail_point(const client& client, document::view test);

///
/// Disables fail points set by tests.
///
void disable_fail_point(const client& client, stdx::string_view failpoint = "failCommand");
void disable_fail_point(std::string uri_string, options::client client_opts);

///
/// Drops the given collection, then recreates it using the jsonSchema
/// from the test, if there is one, and inserts any documents listed
/// in test["data"] into the new collection.
///
void set_up_collection(const client& client, document::view test);

///
/// Deletes all existing documents in the given collection, then inserts
/// all documents passed in initial_data into the collection.
///
void initialize_collection(collection* coll, array::view initial_data);

///
/// Set options from the given operation on the given collection object,
/// including the read concern, write concern, and read preference.
///
void parse_collection_options(document::view op, collection* out);

///
/// Set options from the given operation on the given database object,
/// including the read concern, write concern, and read preference.
///
void parse_database_options(document::view op, database* out);

///
/// Creates an operation_runner via the provided callback, then uses it
/// to run the given operation and check the outcome as per the transactions
/// and client_side_encryption specs.
///
using make_op_runner_fn = std::function<operation_runner()>;
void run_operation_check_result(document::view op, make_op_runner_fn make_op_runner);

///
/// Constructs a uri to the test server that includes options parsed
/// out of test["clientOptions"].
///
uri get_uri(document::view test);

//
// Uses the given environment variable to load all tests in a test suite
// and run them with the provided callback function.
//
using test_runner = std::function<void(const std::string& file)>;
void run_tests_in_suite(std::string ev, test_runner cb);

}  // namespace spec
MONGOCXX_INLINE_NAMESPACE_END
}  // namespace mongocxx
#include <mongocxx/config/private/postlude.hh>
