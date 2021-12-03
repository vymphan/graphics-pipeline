#include "mesh.h"

#include <iostream>
#include <iomanip> // precision()
#include <fstream>
#include <sstream>
#include <string>

// For glm::scale() and glm::translate().
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

namespace {
// Helper functions
}

namespace graphics101 {
void Mesh::computeTangentBitangent() {
    using std::cerr;
    
    if( texcoords.empty() ) {
        cerr << "ERROR: Mesh::computeTangentBitangent() cannot run without texture coordinates.\n";
        return;
    }
    
    assert( face_texcoords.size() == face_positions.size() );
    
    // Clear any existing tangents and bitangents.
    tangents.clear();
    bitangents.clear();
    // Make space for a tangent and bitangent for each position. Initialize them to 0.
    tangents.resize( positions.size(), vec3(0,0,0) );
    bitangents.resize( positions.size(), vec3(0,0,0) );
    // Since there is a tangent for each position, face_tangents should be identical
    // to face_positions.
    face_tangents = face_positions;
    
    // Your code goes here.
    
    // Iterate over faces.
    for ( int i = 0; i <= face_positions.size(); i++ ) {
        // struct t = { A, B, C } == { index1, index2, index3 }
        const vec3 a_object = positions.at( face_positions[i].A );
        const vec3 b_object = positions.at( face_positions[i].B );
        const vec3 c_object = positions.at( face_positions[i].C );

        const vec3 a_texcoord = vec3( texcoords.at( face_texcoords[i].A ), 0. );
        const vec3 b_texcoord = vec3( texcoords.at( face_texcoords[i].B ), 0. );
        const vec3 c_texcoord = vec3( texcoords.at( face_texcoords[i].B ), 0. );

        const vec3 v1 = b_object - a_object;
        const vec3 v2 = c_object - a_object;

        const vec3 u1 = b_texcoord - a_texcoord;
        const vec3 u2 = c_texcoord - a_texcoord;

        // Compute face normal
        const vec3 n_object = normalize( cross( v1, v2 ) );
        const vec3 n_texcoord = normalize( cross( u1, u2 ) );

        mat3 U;
        U[0] = u1;
        U[1] = u2;
        U[2] = n_texcoord;

        mat3 V;
        V[0] = v1;
        V[1] = v2;
        V[2] = n_object;

        mat3 M = V * inverse( U );

        tangents.at( face_tangents[i].A ) += M[0];
        tangents.at( face_tangents[i].B ) += M[0];
        tangents.at( face_tangents[i].C ) += M[0];

        bitangents.at( face_tangents[i].A ) += M[1];
        bitangents.at( face_tangents[i].B ) += M[1];
        bitangents.at( face_tangents[i].C ) += M[1];


    }
    // Compute the known tangent-to-world examples from the triangle edges.
    // Solve for the tangent frame matrix.
    // Average the first column (tangent) and second column (bitangent).
    
    
    // Normalize all vectors.
    for( auto& n : tangents ) { n = normalize(n); }
    for( auto& n : bitangents ) { n = normalize(n); }
}

void Mesh::computeNormals( MeshNormalStrategy strategy ) {
    
    // Clear any existing normals.
    normals.clear();
    // Make space for a normal for each position. Initialize them to 0.
    normals.resize( positions.size(), vec3(0,0,0) );
    // Since there is a normal for each position, face_normals should be identical
    // to face_positions.
    face_normals = face_positions;
    
    // Your code goes here.
    
    // Iterate over faces.
    for ( const Triangle& t : face_positions ) {

        // struct t = { A, B, C } == { index1, index2, index3 }
        const vec3 a = positions.at( t.A ); // x, y, z of vertex A
        const vec3 b = positions.at( t.B ); // x, y, z of vertex B
        const vec3 c = positions.at( t.C ); // x, y, z of vertex C

        vec3 n = normalize( cross( ( a - c ), ( b - c ) ) ) ; // normal

        // tally the normals for each vertex
        normals.at( t.A ) += n;
        normals.at( t.B ) += n;
        normals.at( t.C ) += n;
    }

    // Normalize all normals.
    for ( vec3& n : normals ) {
         n = normalize( n );
    }
    
}

mat4 Mesh::normalizingTransformation() const {
    // Your code goes here.
    
    // If there are no positions, return the identity.
    if( positions.empty() ) {
        return mat4(1.0);
    }
    
    mat4 T = mat4( 1.0 );  // Translation matrix
    mat4 S = mat4( 1.0 );  // Scale matrix

    // Find the minimum and maximum x, y, or z value in the mesh:
    vec3 min = positions.at( 0 );
    vec3 max = positions.at( 0 );

    for ( const vec3& v : positions ) {
        min = glm::min( min, v );
        max = glm::max( max, v );
    }

    // Find mid point
    vec3 mid = vec3( ( min[0] + max[0] ) / 2.0,
                     ( min[1] + max[1] ) / 2.0,
                     ( min[2] + max[2] ) / 2.0 );

    // Build translation matrix
    T = glm::translate( T, vec3( -1 * mid[0],
                                 -1 * mid[1],
                                 -1 * mid[2] ) );

    // The largest difference across x, y, z between max and min
    real maxcoord = glm::max( max[0] - min[0],
                              glm::max( max[1] - min[1],  max[2] - min[2]  ) );

    real scale = 2.0 / maxcoord;

    // Build scale matrix
    S = glm::scale( S, vec3( scale, scale, scale ) );

    return S * T;

}

void Mesh::applyTransformation( const mat4& transform ) {
    // Your code goes here.
    
    // Don't forget to multiply the normals by the inverse-transpose of `mat3(transform)`.
    // Don't forget to normalize() your normals, too.
    
    // A convenient way to iterate over positions with a modifiable reference is:
    // for( vec3& pos : positions ) { pos = ... }
    
    // A convenient way to iterate over normals with a modifiable reference is:
    // for( vec3& n : normals ) { n = ... }

    for ( vec3& pos : positions ) {
        pos = vec3( transform * vec4( pos, 1.0 ) );
    }

    for ( vec3& n : normals ) {
        n = normalize( glm::inverse(glm::transpose( mat3( transform ) ) ) * n );
    }
    
}

}

namespace {
// Helper functions

struct VertexBundle {
    VertexBundle() {}
    VertexBundle( const std::string& str ) { fromString( str ); }
    void fromString( const std::string& str ) {
        std::string v_str, vt_str, vn_str;
        
        // Grab everything up to the first '/'
        auto first_slash = str.find( '/' );
        v_str = str.substr( 0, first_slash );
        
        if( first_slash != std::string::npos ) {
            auto second_slash = str.find( '/', first_slash+1 );
            if( second_slash == std::string::npos ) {
                vt_str = str.substr( first_slash+1 );
            } else {
                vt_str = str.substr( first_slash+1, second_slash-(first_slash+1) );
                vn_str = str.substr( second_slash+1 );
            }
        }
        
        if( !v_str.empty()  ) v  = std::stoi(v_str);
        if( !vt_str.empty() ) vt = std::stoi(vt_str);
        if( !vn_str.empty() ) vn = std::stoi(vn_str);
    }
    
    int v = 0;
    int vt = 0;
    int vn = 0;
};

}

namespace graphics101 {

void Mesh::clear() {
    positions.clear();
    face_positions.clear();
    
    normals.clear();
    face_normals.clear();
    
    texcoords.clear();
    face_texcoords.clear();
}

// Wikipedia has a nice definition of the Wavefront OBJ file format:
//    https://en.wikipedia.org/wiki/Wavefront_.obj_file
bool Mesh::loadFromOBJ( const std::string& path ) {
    using namespace std;
    
    clear();
    
    // TODO: Error checking with a printout to std::cerr and return false.
    
    // Print the number of quads triangulated only once. Keep track of the count.
    int num_quads_triangulated = 0;
    
    // Open the file.
    ifstream mesh( path );
    if( !mesh ) {
        cerr << "ERROR: Unable to access path: " << path << '\n';
        return false;
    }
    while( !( mesh >> ws ).eof() ) {
        // Get each line.
        string line;
        getline( mesh, line );
        
        // Skip blank lines.
        if( line.empty() ) continue;
        
        istringstream linestream( line );
        
        // Get the first word.
        string first_word;
        linestream >> first_word;
        
        if( first_word == "v" ) {
            real x,y,z;
            linestream >> x >> y >> z;
            positions.push_back( vec3( x,y,z ) );
        }
        else if( first_word == "vn" ) {
            real x,y,z;
            linestream >> x >> y >> z;
            normals.push_back( vec3( x,y,z ) );
        }
        else if( first_word == "vt" ) {
            real x,y;
            linestream >> x >> y;
            texcoords.push_back( vec2( x,y ) );
        }
        else if( first_word == "f" ) {
            std::vector< string > fb;
            fb.reserve( 4 );
            while( !( linestream >> ws ).eof() ) {
                string b;
                linestream >> b;
                fb.push_back( b );
            }
            if( fb.size() < 3 ) {
                cerr << "ERROR: Skipping a face with less than 3 vertices.\n";
                continue;
            }
            
            // OBJ files are 1-indexed
            // Negative indices subtract.
            
            std::vector< VertexBundle > vb;
            vb.resize( fb.size() );
            for( int i = 0; i < vb.size(); ++i ) {
                // Convert the string separated by slashes to integers.
                vb[i] = VertexBundle( fb[i] );
                
                // We must have positions, so they can't be zero.
                assert( vb[i].v != 0 );
                
                // If any index is negative, add it to that attribute's length.
                // Otherwise, subtract 1.
                if( vb[i].v  < 0 ) vb[i].v  += positions.size(); else vb[i].v  -= 1;
                if( vb[i].vt < 0 ) vb[i].vt += texcoords.size(); else vb[i].vt -= 1;
                if( vb[i].vn < 0 ) vb[i].vn += normals.size();   else vb[i].vn -= 1;
            }
            
            // We either have no normals/texcoords or we have normal/texcoord faces
            // in correspondence with position faces.
            assert( face_normals.size() == 0 || face_normals.size() == face_positions.size() );
            assert( face_texcoords.size() == 0 || face_texcoords.size() == face_positions.size() );
            
            // Add all the faces.
            if( vb.size() != 3 ) {
                if( vb.size() == 4 ) {
                    num_quads_triangulated += 1;
                } else {
                    cerr << "Triangulating a face with " << vb.size() << " vertices.\n";
                }
            }
            for( int i = 2; i < vb.size(); ++i ) {
                // Add the position face.
                face_positions.emplace_back( vb[0].v, vb[i-1].v, vb[i].v );
                
                // If one vertex bundle has normals or texcoords,
                // they all must have normals/texcoords.
                // We have already converted the OBJ coordinates back to 0-indexing,
                // so check for -1.
                assert( ( vb[0].vn == -1 ) == ( vb[i-1].vn == -1 ) );
                assert( ( vb[0].vn == -1 ) == ( vb[i].vn == -1 ) );
                assert( ( vb[0].vt == -1 ) == ( vb[i-1].vt == -1 ) );
                assert( ( vb[0].vt == -1 ) == ( vb[i].vt == -1 ) );
                
                if( vb[0].vn != -1 ) {
                    face_normals.emplace_back( vb[0].vn, vb[i-1].vn, vb[i].vn );
                }
                
                if( vb[0].vt != -1 ) {
                    face_texcoords.emplace_back( vb[0].vt, vb[i-1].vt, vb[i].vt );
                }
            }
        }
    }
    
    if( num_quads_triangulated > 0 ) {
        cerr << "Triangulated " << num_quads_triangulated << " quadrilaterals.\n";
    }
    
    return true;
}

bool Mesh::writeToOBJ( const std::string& path ) {
    using namespace std;
    
    if( face_normals.size() > 0 && face_normals.size() != face_positions.size() ) {
        cerr << "ERROR: Faces for normals don't match faces for positions. Can't write mesh to OBJ.\n";
        return false;
    }
    if( face_texcoords.size() > 0 && face_texcoords.size() != face_positions.size() ) {
        cerr << "ERROR: Faces for texture coordinates don't match faces for positions. Can't write mesh to OBJ.\n";
        return false;
    }
    
    // Check for valid position indices in face_positions.
    {
        bool valid_indices = true;
        for( const auto& f : face_positions ) {
            for( int vi = 0; vi < 3; ++vi ) {
                if( f[vi] < 0 || f[vi] >= positions.size() ) {
                    valid_indices = false;
                }
            }
        }
        if( !valid_indices ) {
            cerr << "WARNING: Faces for positions contain invalid indices. OBJ will contain an invalid mesh.\n";
            // return false;
        }
    }
    // Check for valid normal indices in face_normals.
    {
        bool valid_indices = true;
        for( const auto& f : face_normals ) {
            for( int vi = 0; vi < 3; ++vi ) {
                if( f[vi] < 0 || f[vi] >= normals.size() ) {
                    valid_indices = false;
                }
            }
        }
        if( !valid_indices ) {
            cerr << "WARNING: Faces for normals contain invalid indices. OBJ will contain an invalid mesh.\n";
            // return false;
        }
    }
    // Check for valid texture coordinate indices in face_texcoords.
    {
        bool valid_indices = true;
        for( const auto& f : face_texcoords ) {
            for( int vi = 0; vi < 3; ++vi ) {
                if( f[vi] < 0 || f[vi] >= texcoords.size() ) {
                    valid_indices = false;
                }
            }
        }
        if( !valid_indices ) {
            cerr << "WARNING: Faces for texture coordinates contain invalid indices. OBJ will contain an invalid mesh.\n";
            // return false;
        }
    }
    
    // Open the file.
    ofstream out( path );
    if( !out ) {
        cerr << "ERROR: Could not open file for writing: " << path << '\n';
        return false;
    }
    
    out << "# OBJ written by Mesh::writeToOBJ()\n";
    out.precision( 32 );
    
    out << '\n';
    for( const auto& v : positions ) {
        out << "v " << v[0] << ' ' << v[1] << ' ' << v[2] << '\n';
    }
    
    out << '\n';
    for( const auto& vt : texcoords ) {
        out << "vt " << vt[0] << ' ' << vt[1] << '\n';
    }
    
    out << '\n';
    for( const auto& vn : normals ) {
        out << "vn " << vn[0] << ' ' << vn[1] << ' ' << vn[2] << '\n';
    }
    
    // OBJ's are 1-indexed, so add one to all indices.
    
    out << '\n';
    if( face_texcoords.empty() && face_normals.empty() ) {
        for( const auto& face : face_positions ) {
            out << "f " << face[0]+1 << ' ' << face[1]+1 << ' ' << face[2]+1 << '\n';
        }
    }
    else if( face_normals.empty() ) {
        for( int face_index = 0; face_index < face_positions.size(); ++face_index ) {
            const auto& p = face_positions.at(face_index);
            const auto& t = face_texcoords.at(face_index);
            out << "f "
                << p[0]+1 << '/' << t[0]+1 << ' '
                << p[1]+1 << '/' << t[1]+1 << ' '
                << p[2]+1 << '/' << t[2]+1 << '\n';
        }
    }
    else if( face_texcoords.empty() ) {
        for( int face_index = 0; face_index < face_positions.size(); ++face_index ) {
            const auto& p = face_positions.at(face_index);
            const auto& n = face_normals.at(face_index);
            out << "f "
                << p[0]+1 << "//" << n[0]+1 << ' '
                << p[1]+1 << "//" << n[1]+1 << ' '
                << p[2]+1 << "//" << n[2]+1 << '\n';
        }
    }
    else {
        for( int face_index = 0; face_index < face_positions.size(); ++face_index ) {
            const auto& p = face_positions.at(face_index);
            const auto& t = face_texcoords.at(face_index);
            const auto& n = face_normals.at(face_index);
            out << "f "
                << p[0]+1 << '/' << t[0]+1 << '/' << n[0]+1 << ' '
                << p[1]+1 << '/' << t[1]+1 << '/' << n[1]+1 << ' '
                << p[2]+1 << '/' << t[2]+1 << '/' << n[2]+1 << '\n';
        }
    }

    return true;
}

}
