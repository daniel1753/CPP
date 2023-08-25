/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: function.cpp					
 * ******************************/
#include <memory>  /* shared_ptr */
#include <cstdlib> /* new */
#pragma once 

namespace ilrd
{

/* forward declaration */
template<typename>
class Function;


/******************************************************************************/
/********************************* Function ***********************************/
/********************************* with out ***********************************/
/************************ invokationargs (with bing arg) **********************/
/******************************************************************************/

template<typename RET>
class Function<RET(void)>
{
public:
    /* defult ctor */
    explicit Function(){}
    
    /* FreeFunction override */
    template<typename BIND_ARG>
    explicit Function(RET (*callable)(BIND_ARG arg), BIND_ARG arg);
    
    /* MemberFunction override */
    template< typename T, typename BIND_ARG>
    explicit Function(RET (T::*callable)(BIND_ARG arg), T *object,BIND_ARG arg);
    
    RET operator()();    //Will Activate derived RUN();
      
private:


    class IFunction;
    std::shared_ptr<IFunction> m_ifunction;
    
    template<typename BIND_ARG>
    class FreeFunction;
    
    template<typename T, typename BIND_ARG>
    class MemberFunction;
    
};


/*//////////////////////////////// IFunction \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template<typename RET>
class Function<RET(void)>::IFunction
{
public:
    virtual ~IFunction(){};
    virtual RET Run() = 0;   /* pure virtual */
};

/*/////////////////////////////// FreeFunction \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/* templete inside templete */
template<typename RET>
template<typename BIND_ARG>
class Function<RET(void)>::FreeFunction: public IFunction
{
public:
    explicit FreeFunction(RET (*callable)(BIND_ARG arg), BIND_ARG arg);
    RET Run() override;

private:
    RET (*m_callable)(BIND_ARG arg);
    BIND_ARG m_arg;
};

/*///////////////////////////// MemberFunction \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template<typename RET>
template<typename T, typename BIND_ARG>
class Function<RET(void)>::MemberFunction: public IFunction
{
public:
    explicit MemberFunction(RET (T::*callable)(BIND_ARG arg), 
                            T* object, 
                            BIND_ARG arg);
    RET Run() override;

private:
    RET (T::*m_callable)(BIND_ARG arg);
    T* m_object;
    BIND_ARG m_arg;
};

/*////////////////////////////// Function impl \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template <typename RET>
template< typename T,typename BIND_ARG>
inline Function<RET(void)>::Function(RET (T::*callable)(BIND_ARG arg), 
                              T *object, 
                              BIND_ARG arg):
                              m_ifunction(new MemberFunction<T, BIND_ARG>(
                                                                    callable, 
                                                                    object, 
                                                                    arg))
{}

template <typename RET>
template<typename BIND_ARG>
inline Function<RET(void)>::Function(
                                RET (*callable)(BIND_ARG arg), 
                                BIND_ARG arg):
                                m_ifunction(new FreeFunction<BIND_ARG>(callable, 
                                                                       arg))
{}
    
template<typename RET>
inline RET Function<RET(void)>::operator()()
{
    return m_ifunction->Run();
}


/*/////////////////////////// FreeFunction impl \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template<typename RET>
template<typename BIND_ARG>
inline Function<RET(void)>::FreeFunction<BIND_ARG>::FreeFunction(
                                                RET(* callable)(BIND_ARG arg), 
                                                BIND_ARG arg):
                                                m_callable(callable),
                                                m_arg(arg)
{}


template<typename RET>
template<typename BIND_ARG>
inline RET Function<RET(void)>::FreeFunction<BIND_ARG>::Run()
{
    return m_callable(m_arg);
}

/*///////////////////////// MemberFunction impl \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template<typename RET>
template<typename T, typename BIND_ARG>
inline Function<RET(void)>::MemberFunction<T,BIND_ARG>::MemberFunction(
                                                RET(T::*callable)(BIND_ARG arg), 
                                                T* object, 
                                                BIND_ARG arg):
                                                m_callable(callable),
                                                m_object(object),
                                                m_arg(arg){}


template<typename RET>
template<typename T, typename BIND_ARG>
inline RET Function<RET(void)>::MemberFunction<T,BIND_ARG>::Run()
{
    return (m_object->*m_callable)(m_arg);
}

/******************************************************************************/
/********************************** Function **********************************/
/**********************************   with   **********************************/
/****************************    invokation args   ****************************/
/******************************************************************************/


template<typename RET, typename INVOCATION_ARG>
class Function<RET(INVOCATION_ARG)>
{
public:
    /* defult ctor */
    explicit Function(){}
    
    /* FreeFunction override */
    explicit Function(RET (*callable)(INVOCATION_ARG ));
    
    /* MemberFunction override */
    template< typename T>
    explicit Function(RET (T::*callable)(INVOCATION_ARG), T *object);
    
    RET operator()(INVOCATION_ARG arg) const;    //Will Activate derived RUN();
      
private:


    class IFunction;
    std::shared_ptr<IFunction> m_ifunction;
    
    class FreeFunction;
    
    template<typename T>
    class MemberFunction;
    
};


/*//////////////////////////////// IFunction \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template<typename RET, typename INVOCATION_ARG>
class Function<RET(INVOCATION_ARG)>::IFunction
{
public:
    virtual ~IFunction(){};
    virtual RET Run(INVOCATION_ARG arg) = 0;   /* pure virtual */
};

/*/////////////////////////////// FreeFunction \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/* templete inside templete */
template<typename RET, typename INVOCATION_ARG>
class Function<RET(INVOCATION_ARG)>::FreeFunction: public IFunction
{
public:
    explicit FreeFunction(RET (*callable)(INVOCATION_ARG arg));
    RET Run(INVOCATION_ARG arg) override;

private:
    RET (*m_callable)(INVOCATION_ARG arg);
};

/*///////////////////////////// MemberFunction \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template<typename RET, typename INVOCATION_ARG>
template<typename T>
class Function<RET(INVOCATION_ARG)>::MemberFunction: public IFunction
{
public:
    explicit MemberFunction(RET (T::*callable)(INVOCATION_ARG arg), T* object);
    RET Run(INVOCATION_ARG arg) override;

private:
    RET (T::*m_callable)(INVOCATION_ARG arg);
    T* m_object;
    
};



/*////////////////////////////// Function impl \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template <typename RET,typename INVOCATION_ARG>
template< typename T>
inline Function<RET(INVOCATION_ARG)>::Function(
                                              RET(T::*callable)(INVOCATION_ARG), 
                                              T *object):
                                              m_ifunction(
                                                new MemberFunction<T>(
                                                                      callable, 
                                                                      object))
{}

template <typename RET,typename INVOCATION_ARG>
inline Function<RET(INVOCATION_ARG)>::Function(RET (*callable)(INVOCATION_ARG)):
                              m_ifunction(new FreeFunction(callable))
{}


template <typename RET, typename INVOCATION_ARG>
inline RET Function<RET(INVOCATION_ARG)>::operator()(INVOCATION_ARG arg) const
{
    return m_ifunction->Run(arg);
}

/*///////////////////////// MemberFunction impl \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template <typename RET,typename INVOCATION_ARG>
template<typename T>
inline Function<RET(INVOCATION_ARG)>::MemberFunction<T>::MemberFunction(
                                        RET (T::*callable)(INVOCATION_ARG arg), 
                                        T* object):
                                        m_callable(callable),
                                        m_object(object){}


template <typename RET,typename INVOCATION_ARG>
template<typename T>
inline RET Function<RET(INVOCATION_ARG)>::MemberFunction<T>::Run(
                                                             INVOCATION_ARG arg)
{
    return (m_object->*m_callable)(arg);
}


/*/////////////////////////// FreeFunction impl \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template <typename RET,typename INVOCATION_ARG>
inline Function<RET(INVOCATION_ARG)>::FreeFunction::FreeFunction(
                                           RET(* callable)(INVOCATION_ARG arg)):
                                           m_callable(callable)
                                               
{}


template<typename RET,typename INVOCATION_ARG>
inline RET Function<RET(INVOCATION_ARG)>::FreeFunction::Run(INVOCATION_ARG arg)
{
    return m_callable(arg);
}


} // namespace ilrd