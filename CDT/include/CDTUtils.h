/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

/**
 * @file
 * Utilities and helpers
 */

#ifndef CDT_obwOaxOTdAWcLNTlNnaq
#define CDT_obwOaxOTdAWcLNTlNnaq

#ifdef CDT_DONT_USE_BOOST_RTREE
// CDT_DONT_USE_BOOST_RTREE was replaced with CDT_USE_BOOST
typedef char CDT_DONT_USE_BOOST_RTREE__was__replaced__with__CDT_USE_BOOST[-1];
#endif

// #define CDT_USE_STRONG_TYPING // strong type checks on indices

// check if c++11 is supported
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
#define CDT_CXX11_IS_SUPPORTED
#elif !defined(__cplusplus) && !defined(_MSC_VER)
typedef char couldnt_parse_cxx_standard[-1]; ///< Error: couldn't parse standard
#endif

// Functions defined outside the class need to be 'inline'
// if CDT is configured to be used as header-only library:
// single-definition rule is violated otherwise
#ifdef CDT_USE_AS_COMPILED_LIBRARY
#define CDT_INLINE_IF_HEADER_ONLY
#include "cdt_export.h" // automatically generated by CMake
#else
/**
 * Macro for inlining non-template functions when in header-only mode to
 * avoid multiple declaration errors.
 */
#define CDT_INLINE_IF_HEADER_ONLY inline
/// Export not needed in header-only mode
#define CDT_EXPORT
#endif

#include <cassert>
#include <cmath>
#include <limits>
#include <vector>

#ifdef CDT_USE_BOOST
#include <boost/container/flat_set.hpp>
#endif

// use fall-backs for c++11 features
#ifdef CDT_CXX11_IS_SUPPORTED

#include <array>
#include <functional>
#include <random>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
namespace CDT
{
using std::array;
using std::get;
using std::make_tuple;
using std::mt19937;
using std::tie;
using std::tuple;
using std::unordered_map;
using std::unordered_set;
} // namespace CDT

#else
#include <boost/array.hpp>
#include <boost/functional/hash.hpp>
#include <boost/random.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
namespace CDT
{
using boost::array;
using boost::get;
using boost::make_tuple;
using boost::tie;
using boost::tuple;
using boost::unordered_map;
using boost::unordered_set;
using boost::random::mt19937;
} // namespace CDT
#endif

namespace CDT
{

/// 2D vector
template <typename T>
struct CDT_EXPORT V2d
{
    T x; ///< X-coordinate
    T y; ///< Y-coordinate

    /// Create vector from X and Y coordinates
    static V2d make(T x, T y);
};

/// X- coordinate getter for V2d
template <typename T>
const T& getX_V2d(const V2d<T>& v)
{
    return v.x;
}

/// Y-coordinate getter for V2d
template <typename T>
const T& getY_V2d(const V2d<T>& v)
{
    return v.y;
}

/// If two 2D vectors are exactly equal
template <typename T>
bool operator==(const CDT::V2d<T>& lhs, const CDT::V2d<T>& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

#ifdef CDT_USE_64_BIT_INDEX_TYPE
typedef unsigned long long IndexSizeType;
#else
typedef unsigned int IndexSizeType;
#endif

#ifndef CDT_USE_STRONG_TYPING
/// Index in triangle
typedef unsigned char Index;
/// Vertex index
typedef IndexSizeType VertInd;
/// Triangle index
typedef IndexSizeType TriInd;
#else
/// Index in triangle
BOOST_STRONG_TYPEDEF(unsigned char, Index);
/// Vertex index
BOOST_STRONG_TYPEDEF(IndexSizeType, VertInd);
/// Triangle index
BOOST_STRONG_TYPEDEF(IndexSizeType, TriInd);
#endif

/// Constant representing no valid neighbor for a triangle
const static TriInd noNeighbor(std::numeric_limits<TriInd>::max());
/// Constant representing no valid vertex for a triangle
const static VertInd noVertex(std::numeric_limits<VertInd>::max());

typedef std::vector<TriInd> TriIndVec;  ///< Vector of triangle indices
typedef array<VertInd, 3> VerticesArr3; ///< array of three vertex indices
typedef array<TriInd, 3> NeighborsArr3; ///< array of three neighbors

/// 2D bounding box
template <typename T>
struct CDT_EXPORT Box2d
{
    V2d<T> min; ///< min box corner
    V2d<T> max; ///< max box corner

    /// Envelop box around a point
    void envelopPoint(const V2d<T>& p)
    {
        envelopPoint(p.x, p.y);
    }
    /// Envelop box around a point with given coordinates
    void envelopPoint(const T x, const T y)
    {
        min.x = std::min(x, min.x);
        max.x = std::max(x, max.x);
        min.y = std::min(y, min.y);
        max.y = std::max(y, max.y);
    }
};

/// Bounding box of a collection of custom 2D points given coordinate getters
template <
    typename T,
    typename TVertexIter,
    typename TGetVertexCoordX,
    typename TGetVertexCoordY>
Box2d<T> envelopBox(
    TVertexIter first,
    TVertexIter last,
    TGetVertexCoordX getX,
    TGetVertexCoordY getY)
{
    const T max = std::numeric_limits<T>::max();
    Box2d<T> box = {{max, max}, {-max, -max}};
    for(; first != last; ++first)
    {
        box.envelopPoint(getX(*first), getY(*first));
    }
    return box;
}

/// Bounding box of a collection of 2D points
template <typename T>
CDT_EXPORT Box2d<T> envelopBox(const std::vector<V2d<T> >& vertices);

/// Edge connecting two vertices: vertex with smaller index is always first
/// \note: hash Edge is specialized at the bottom
struct CDT_EXPORT Edge
{
    /// Constructor
    Edge(VertInd iV1, VertInd iV2);
    /// Equals operator
    bool operator==(const Edge& other) const;
    /// Not-equals operator
    bool operator!=(const Edge& other) const;
    /// V1 getter
    VertInd v1() const;
    /// V2 getter
    VertInd v2() const;
    /// Edges' vertices
    const std::pair<VertInd, VertInd>& verts() const;

private:
    std::pair<VertInd, VertInd> m_vertices;
};

/// Get edge first vertex
inline VertInd edge_get_v1(const Edge& e)
{
    return e.v1();
}

/// Get edge second vertex
inline VertInd edge_get_v2(const Edge& e)
{
    return e.v2();
}

/// Get edge second vertex
inline Edge edge_make(VertInd iV1, VertInd iV2)
{
    return Edge(iV1, iV2);
}

typedef std::vector<Edge> EdgeVec;                ///< Vector of edges
typedef unordered_set<Edge> EdgeUSet;             ///< Hash table of edges
typedef unordered_set<TriInd> TriIndUSet;         ///< Hash table of triangles
typedef unordered_map<TriInd, TriInd> TriIndUMap; ///< Triangle hash map
#ifdef CDT_USE_BOOST
/// Flat hash table of triangles
typedef boost::container::flat_set<TriInd> TriIndFlatUSet;
#endif

/// Triangulation triangle (CCW winding)
/* Counter-clockwise winding:
       v3
       /\
    n3/  \n2
     /____\
   v1  n1  v2                 */
struct CDT_EXPORT Triangle
{
    VerticesArr3 vertices;   ///< triangle's three vertices
    NeighborsArr3 neighbors; ///< triangle's three neighbors

    /**
     * Factory method
     * @note needed for c++03 compatibility (no uniform initialization
     * available)
     */
    static Triangle
    make(const array<VertInd, 3>& vertices, const array<TriInd, 3>& neighbors)
    {
        Triangle t = {vertices, neighbors};
        return t;
    }
};

typedef std::vector<Triangle> TriangleVec; ///< Vector of triangles

/// Advance vertex or neighbor index counter-clockwise
CDT_EXPORT Index ccw(Index i);

/// Advance vertex or neighbor index clockwise
CDT_EXPORT Index cw(Index i);

/// Location of point on a triangle
struct CDT_EXPORT PtTriLocation
{
    /// Enum
    enum Enum
    {
        Inside,
        Outside,
        OnEdge1,
        OnEdge2,
        OnEdge3,
    };
};

/// Check if location is classified as on any of three edges
CDT_EXPORT bool isOnEdge(PtTriLocation::Enum location);

/// Neighbor index from a on-edge location
/// \note Call only if located on the edge!
CDT_EXPORT Index edgeNeighbor(PtTriLocation::Enum location);

/// Relative location of point to a line
struct CDT_EXPORT PtLineLocation
{
    /// Enum
    enum Enum
    {
        Left,
        Right,
        OnLine,
    };
};

/// Orient p against line v1-v2 2D: robust geometric predicate
template <typename T>
CDT_EXPORT T orient2D(const V2d<T>& p, const V2d<T>& v1, const V2d<T>& v2);

/// Check if point lies to the left of, to the right of, or on a line
template <typename T>
CDT_EXPORT PtLineLocation::Enum locatePointLine(
    const V2d<T>& p,
    const V2d<T>& v1,
    const V2d<T>& v2,
    T orientationTolerance = T(0));

/// Classify value of orient2d predicate
template <typename T>
CDT_EXPORT PtLineLocation::Enum
classifyOrientation(T orientation, T orientationTolerance = T(0));

/// Check if point a lies inside of, outside of, or on an edge of a triangle
template <typename T>
CDT_EXPORT PtTriLocation::Enum locatePointTriangle(
    const V2d<T>& p,
    const V2d<T>& v1,
    const V2d<T>& v2,
    const V2d<T>& v3);

/// Opposed neighbor index from vertex index
CDT_EXPORT CDT_INLINE_IF_HEADER_ONLY Index opoNbr(Index vertIndex);

/// Opposed vertex index from neighbor index
CDT_EXPORT CDT_INLINE_IF_HEADER_ONLY Index opoVrt(Index neighborIndex);

/// Index of triangle's neighbor opposed to a vertex
CDT_EXPORT CDT_INLINE_IF_HEADER_ONLY Index
opposedTriangleInd(const Triangle& tri, VertInd iVert);

/// Index of triangle's neighbor opposed to an edge
CDT_INLINE_IF_HEADER_ONLY Index
opposedTriangleInd(const Triangle& tri, VertInd iVedge1, VertInd iVedge2);

/// Index of triangle's vertex opposed to a triangle
CDT_EXPORT CDT_INLINE_IF_HEADER_ONLY Index
opposedVertexInd(const Triangle& tri, TriInd iTopo);

/// If triangle has a given neighbor return neighbor-index, throw otherwise
CDT_EXPORT CDT_INLINE_IF_HEADER_ONLY Index
neighborInd(const Triangle& tri, TriInd iTnbr);

/// If triangle has a given vertex return vertex-index, throw otherwise
CDT_EXPORT CDT_INLINE_IF_HEADER_ONLY Index
vertexInd(const Triangle& tri, VertInd iV);

/// Given triangle and a vertex find opposed triangle
CDT_EXPORT CDT_INLINE_IF_HEADER_ONLY TriInd
opposedTriangle(const Triangle& tri, VertInd iVert);

/// Given two triangles, return vertex of first triangle opposed to the second
CDT_EXPORT CDT_INLINE_IF_HEADER_ONLY VertInd
opposedVertex(const Triangle& tri, TriInd iTopo);

/// Test if point lies in a circumscribed circle of a triangle
template <typename T>
CDT_EXPORT bool isInCircumcircle(
    const V2d<T>& p,
    const V2d<T>& v1,
    const V2d<T>& v2,
    const V2d<T>& v3);

/// Test if two vertices share at least one common triangle
CDT_EXPORT CDT_INLINE_IF_HEADER_ONLY bool
verticesShareEdge(const TriIndVec& aTris, const TriIndVec& bTris);

/// Distance between two 2D points
template <typename T>
CDT_EXPORT T distance(const V2d<T>& a, const V2d<T>& b);

/// Squared distance between two 2D points
template <typename T>
CDT_EXPORT T distanceSquared(const V2d<T>& a, const V2d<T>& b);

} // namespace CDT

#ifndef CDT_USE_AS_COMPILED_LIBRARY
#include "CDTUtils.hpp"
#endif

//*****************************************************************************
// Specialize hash functions
//*****************************************************************************
#ifdef CDT_CXX11_IS_SUPPORTED
namespace std
#else
namespace boost
#endif
{

#ifdef CDT_USE_STRONG_TYPING

/// Vertex index hasher
template <>
struct hash<CDT::VertInd>
{
    /// Hash operator
    std::size_t operator()(const CDT::VertInd& vi) const
    {
        return std::hash<std::size_t>()(vi.t);
    }
};

/// Triangle index hasher
template <>
struct hash<CDT::TriInd>
{
    /// Hash operator
    std::size_t operator()(const CDT::TriInd& vi) const
    {
        return std::hash<std::size_t>()(vi.t);
    }
};

#endif // CDT_USE_STRONG_TYPING

/// Edge hasher
template <>
struct hash<CDT::Edge>
{
    /// Hash operator
    std::size_t operator()(const CDT::Edge& e) const
    {
        return hashEdge(e);
    }

private:
    static void hashCombine(std::size_t& seed, const CDT::VertInd& key)
    {
#ifdef CDT_CXX11_IS_SUPPORTED
        typedef std::hash<CDT::VertInd> Hasher;
#else
        typedef boost::hash<CDT::VertInd> Hasher;
#endif
        seed ^= Hasher()(key) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    static std::size_t hashEdge(const CDT::Edge& e)
    {
        const std::pair<CDT::VertInd, CDT::VertInd>& vv = e.verts();
        std::size_t seed1(0);
        hashCombine(seed1, vv.first);
        hashCombine(seed1, vv.second);
        std::size_t seed2(0);
        hashCombine(seed2, vv.second);
        hashCombine(seed2, vv.first);
        return std::min(seed1, seed2);
    }
};
} // namespace std/boost

#endif // header guard
