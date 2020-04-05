#include <MatrixOpe.h>



namespace MLL
{
    template<typename,elemType>
    class Model 
    {
        public:
            std::vector<std::vector<elemType> > train_data;
            std::vector<std::vector<elemType> > test_data;
        protected:
            Model()
            {
                std::cout << "base class structor" << std::edl;   
            }
            virture ~Model() 
            {
                std::cout << "base class destructor" << std::edl;   
            }
            template<typename,elemType>
            train(std::vector<std::vector<elemType> > train_data)
            {

            }
            template<typename,elemType>
            test(std::vector<std::vector<elemType> > test_data)
            {

            }
    }
}

