#ifndef VISIBILITY_TESTER_HPP
#define VISIBILITY_TESTER_HPP


#ifndef SCENES_HPP
    namespace rt{
        class VisibilityTester;
    }
    #include "scenes.hpp"
#endif  //< SCENES_HPP

#include "surfel.hpp"

namespace rt{

    class VisibilityTester {
        private:
            Surfel p0, p1;  //< Test visibility between p0 and p1.
            Vec3 direction; //< Direction to test visibility from p0.
        public:
            VisibilityTester() = default;
            VisibilityTester( const Surfel&, const Surfel& );
            VisibilityTester( const Surfel&, const Vec3&); // For directional
            bool unoccluded( const Scene& scene );
        
        };

}


#endif //< VISIBILITY_TESTER_HPP