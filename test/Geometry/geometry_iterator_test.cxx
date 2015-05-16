/**
 * @file   geometry_iterator_test.cxx
 * @brief  Unit test for geometry iterators on a standard detector
 * @date   May 7th, 2015
 * @author petrillo@fnal.gov
 * 
 * Usage: just run the executable.
 * Boost unit testing environment keeps the arguments secret anyway.
 */

// LArSoft libraries
#include "test/Geometry/geometry_unit_test_base.h"
#include "test/Geometry/GeometryIteratorTestAlg.h"
#include "Geometry/GeometryCore.h"
#include "Geometry/ChannelMapStandardAlg.h"

// utility libraries
#include "messagefacility/MessageLogger/MessageLogger.h"

// Boost libraries
#define BOOST_TEST_MODULE GeometryIteratorTest
#include <boost/test/included/unit_test.hpp>

// C/C++ standard libraries
#include <string>

//------------------------------------------------------------------------------
//---  The test environment
//---

// we define here all the configuration that is needed;
// in the specific, the type of the channel mapping and a proper test name,
// used for output only; we use BasicGeometryFixtureConfigurer as base class
// because it's easy, but we could use any new unrelated class as long as it
// provides what testing::GeometryTesterFixture needs.
struct StandardGeometryConfiguration:
  public testing::BasicGeometryFixtureConfigurer<geo::ChannelMapStandardAlg>
{
  /// Constructor: overrides the application name
  StandardGeometryConfiguration()
    { SetApplicationName("GeometryIteratorUnitTest"); }
}; // class StandardGeometryConfiguration

/*
 * Our fixture is based on GeometryTesterFixture, configured with the object
 * above.
 * It provides to the testing environment:
 * - `Tester()`, returning a configured instance of the test algorithm;
 * - `GlobalTester()`, (static) returning a global configured instance of the
 *   test algorithm.
 * 
 * The testing::TestSharedGlobalResource<> facility provides a singleton
 * instance of the tester algorithm, shared through the whole program and with
 * this object too.
 * 
 * This sharing allows the fixture to be used either as global or as per-suite.
 * In the former case, the BOOST_AUTO_TEST_CASE's will access the global test
 * algotithm instance through the static call to
 * `GeometryIteratorTestFixture::GlobalTester()`; in the latter case, it will
 * access the local tester via the member function `Tester()`.
 * In this case, whether `GlobalTester()` and `Tester()` point to the same
 * tester depends on Boost unit test implementation.
 */
class GeometryIteratorTestFixture:
  private testing::GeometryTesterFixture<StandardGeometryConfiguration>
{
  using Tester_t = geo::GeometryIteratorTestAlg;
  
  using TesterRegistry_t = testing::TestSharedGlobalResource<Tester_t>;

    public:
  
  /// Constructor: initialize the tester with the Geometry from base class
  GeometryIteratorTestFixture()
    {
      // create a new tester
      tester_ptr = std::make_shared<Tester_t>(TesterConfiguration());
      tester_ptr->Setup(*Geometry()); // Geometry() is inherited
      // if no tester is default yet, share ours:
      TesterRegistry_t::ProvideDefaultSharedResource(tester_ptr);
    }
  
  /// Retrieves the local tester
  Tester_t& Tester() { return *(tester_ptr.get()); }
  
  /// Retrieves the global tester
  static Tester_t& GlobalTester() { return TesterRegistry_t::Resource(); }
  
    private:
  std::shared_ptr<Tester_t> tester_ptr; ///< our tester (may be shared)
}; // class GeometryIteratorTestFixture



//------------------------------------------------------------------------------
//---  The tests
//---
//
// Note on Boost fixture options:
// - BOOST_FIXTURE_TEST_SUITE will provide the tester as a always-present data
//   member in the environment, as "Tester()"; but a new fixture, with a new
//   geometry and a new tester, is initialized on each test case
// - BOOST_GLOBAL_FIXTURE does not provide tester access, so one has to get it
//   as GeometryIteratorTestFixture::GlobalTester(); on the other hand, the
//   fixture is initialized only when a new global one is explicitly created.
//

// BOOST_FIXTURE_TEST_SUITE(GeometryIterators, GeometryIteratorTestFixture)
BOOST_GLOBAL_FIXTURE(GeometryIteratorTestFixture)

/*
BOOST_AUTO_TEST_CASE( AllTests )
{
  GlobalTester().Run();
} // BOOST_AUTO_TEST_CASE( AllTests )
*/

BOOST_AUTO_TEST_CASE( CryostatIteratorsTest )
{
  GeometryIteratorTestFixture::GlobalTester().CryostatIteratorsTest();
} // BOOST_AUTO_TEST_CASE( CryostatIteratorsTest )



BOOST_AUTO_TEST_CASE( TPCIteratorsTest )
{
  GeometryIteratorTestFixture::GlobalTester().TPCIteratorsTest();
} // BOOST_AUTO_TEST_CASE( TPCIteratorsTest )



BOOST_AUTO_TEST_CASE( PlaneIteratorsTest )
{
  GeometryIteratorTestFixture::GlobalTester().PlaneIteratorsTest();
} // BOOST_AUTO_TEST_CASE( PlaneIteratorsTest )



BOOST_AUTO_TEST_CASE( WireIteratorsTest )
{
  GeometryIteratorTestFixture::GlobalTester().WireIteratorsTest();
} // BOOST_AUTO_TEST_CASE( WireIteratorsTest )

// BOOST_AUTO_TEST_SUITE_END()