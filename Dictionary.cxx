//
// File generated by rootcint at Mon Dec  6 12:02:38 2010

// Do NOT change. Changes will be lost next time file is generated
//

#include "RConfig.h" //rootcint 4834
#if !defined(R__ACCESS_IN_SYMBOL)
//Break the privacy of classes -- Disabled for the moment
#define private public
#define protected public
#endif

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;
#include "Dictionary.h"

#include "TClass.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"

// START OF SHADOWS

namespace ROOT {
   namespace Shadow {
   } // of namespace Shadow
} // of namespace ROOT
// END OF SHADOWS

namespace ROOT {
   void NGammaEvent_ShowMembers(void *obj, TMemberInspector &R__insp, char *R__parent);
   static void *new_NGammaEvent(void *p = 0);
   static void *newArray_NGammaEvent(Long_t size, void *p);
   static void delete_NGammaEvent(void *p);
   static void deleteArray_NGammaEvent(void *p);
   static void destruct_NGammaEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NGammaEvent*)
   {
      ::NGammaEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NGammaEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("NGammaEvent", ::NGammaEvent::Class_Version(), "NGammaEvent.h", 7,
                  typeid(::NGammaEvent), DefineBehavior(ptr, ptr),
                  &::NGammaEvent::Dictionary, isa_proxy, 0,
                  sizeof(::NGammaEvent) );
      instance.SetNew(&new_NGammaEvent);
      instance.SetNewArray(&newArray_NGammaEvent);
      instance.SetDelete(&delete_NGammaEvent);
      instance.SetDeleteArray(&deleteArray_NGammaEvent);
      instance.SetDestructor(&destruct_NGammaEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NGammaEvent*)
   {
      return GenerateInitInstanceLocal((::NGammaEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::NGammaEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
TClass *NGammaEvent::fgIsA = 0;  // static to hold class pointer

//______________________________________________________________________________
const char *NGammaEvent::Class_Name()
{
   return "NGammaEvent";
}

//______________________________________________________________________________
const char *NGammaEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NGammaEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int NGammaEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NGammaEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void NGammaEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NGammaEvent*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *NGammaEvent::Class()
{
   if (!fgIsA) fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NGammaEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
void NGammaEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class NGammaEvent.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> channel;
      R__b >> evtnum;
      R__b >> triggerTime;
      R__b >> peakTime;
      R__b >> centerDelta;
      R__b >> peakTimeInterp;
      R__b >> pedestal;
      R__b >> minSample;
      R__b >> maxSample;
      R__b >> maxInterp;
      R__b >> area;
      R__b >> t1;
      R__b >> dt;
      R__b >> p1;
      R__b >> p2;
      R__b >> dEdx;
      R__b >> dEdx_tail;
      R__b >> dEdx_min;
      R__b >> chi2;
      R__b.CheckByteCount(R__s, R__c, NGammaEvent::IsA());
   } else {
      R__c = R__b.WriteVersion(NGammaEvent::IsA(), kTRUE);
      R__b << channel;
      R__b << evtnum;
      R__b << triggerTime;
      R__b << peakTime;
      R__b << centerDelta;
      R__b << peakTimeInterp;
      R__b << pedestal;
      R__b << minSample;
      R__b << maxSample;
      R__b << maxInterp;
      R__b << area;
      R__b << t1;
      R__b << dt;
      R__b << p1;
      R__b << p2;
      R__b << dEdx;
      R__b << dEdx_tail;
      R__b << dEdx_min;
      R__b << chi2;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void NGammaEvent::ShowMembers(TMemberInspector &R__insp, char *R__parent)
{
      // Inspect the data members of an object of class NGammaEvent.
      TClass *R__cl = ::NGammaEvent::IsA();
      Int_t R__ncp = strlen(R__parent);
      if (R__ncp || R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__parent, "channel", &channel);
      R__insp.Inspect(R__cl, R__parent, "evtnum", &evtnum);
      R__insp.Inspect(R__cl, R__parent, "triggerTime", &triggerTime);
      R__insp.Inspect(R__cl, R__parent, "peakTime", &peakTime);
      R__insp.Inspect(R__cl, R__parent, "centerDelta", &centerDelta);
      R__insp.Inspect(R__cl, R__parent, "peakTimeInterp", &peakTimeInterp);
      R__insp.Inspect(R__cl, R__parent, "pedestal", &pedestal);
      R__insp.Inspect(R__cl, R__parent, "minSample", &minSample);
      R__insp.Inspect(R__cl, R__parent, "maxSample", &maxSample);
      R__insp.Inspect(R__cl, R__parent, "maxInterp", &maxInterp);
      R__insp.Inspect(R__cl, R__parent, "area", &area);
      R__insp.Inspect(R__cl, R__parent, "t1", &t1);
      R__insp.Inspect(R__cl, R__parent, "dt", &dt);
      R__insp.Inspect(R__cl, R__parent, "p1", &p1);
      R__insp.Inspect(R__cl, R__parent, "p2", &p2);
      R__insp.Inspect(R__cl, R__parent, "dEdx", &dEdx);
      R__insp.Inspect(R__cl, R__parent, "dEdx_tail", &dEdx_tail);
      R__insp.Inspect(R__cl, R__parent, "dEdx_min", &dEdx_min);
      R__insp.Inspect(R__cl, R__parent, "chi2", &chi2);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NGammaEvent(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::NGammaEvent : new ::NGammaEvent;
   }
   static void *newArray_NGammaEvent(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::NGammaEvent[nElements] : new ::NGammaEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_NGammaEvent(void *p) {
      delete ((::NGammaEvent*)p);
   }
   static void deleteArray_NGammaEvent(void *p) {
      delete [] ((::NGammaEvent*)p);
   }
   static void destruct_NGammaEvent(void *p) {
      typedef ::NGammaEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NGammaEvent

/********************************************************
* Dictionary.cxx
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************/

#ifdef G__MEMTEST
#undef malloc
#undef free
#endif

#if defined(__GNUC__) && (__GNUC__ > 3) && (__GNUC_MINOR__ > 1)
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

extern "C" void G__cpp_reset_tagtableDictionary();

extern "C" void G__set_cpp_environmentDictionary() {
  G__add_compiledheader("TObject.h");
  G__add_compiledheader("TMemberInspector.h");
  G__add_compiledheader("NGammaEvent.h");
  G__cpp_reset_tagtableDictionary();
}
#include <new>
extern "C" int G__cpp_dllrevDictionary() { return(30051515); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* NGammaEvent */
static int G__Dictionary_189_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) NGammaEvent::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__Dictionary_189_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) NGammaEvent::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__Dictionary_189_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) NGammaEvent::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__Dictionary_189_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      NGammaEvent::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__Dictionary_189_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) ((const NGammaEvent*) G__getstructoffset())->IsA());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__Dictionary_189_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((NGammaEvent*) G__getstructoffset())->ShowMembers(*(TMemberInspector*) libp->para[0].ref, (char*) G__int(libp->para[1]));
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__Dictionary_189_0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((NGammaEvent*) G__getstructoffset())->Streamer(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__Dictionary_189_0_8(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((NGammaEvent*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__Dictionary_189_0_9(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) NGammaEvent::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__Dictionary_189_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) NGammaEvent::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__Dictionary_189_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) NGammaEvent::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__Dictionary_189_0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) NGammaEvent::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic default constructor
static int G__Dictionary_189_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   NGammaEvent *p;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new NGammaEvent[n];
     } else {
       p = new((void*) gvp) NGammaEvent[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new NGammaEvent;
     } else {
       p = new((void*) gvp) NGammaEvent;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__DictionaryLN_NGammaEvent));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__Dictionary_189_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   NGammaEvent* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new NGammaEvent(*(NGammaEvent*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__DictionaryLN_NGammaEvent));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef NGammaEvent G__TNGammaEvent;
static int G__Dictionary_189_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 0
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (NGammaEvent*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((NGammaEvent*) (soff+(sizeof(NGammaEvent)*i)))->~G__TNGammaEvent();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (NGammaEvent*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((NGammaEvent*) (soff))->~G__TNGammaEvent();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__Dictionary_189_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   NGammaEvent* dest = (NGammaEvent*) G__getstructoffset();
   *dest = *(NGammaEvent*) libp->para[0].ref;
   const NGammaEvent& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Setting up global function */

/*********************************************************
* Member function Stub
*********************************************************/

/* NGammaEvent */

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncDictionary {
 public:
  G__Sizep2memfuncDictionary(): p(&G__Sizep2memfuncDictionary::sizep2memfunc) {}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncDictionary::*p)();
};

size_t G__get_sizep2memfuncDictionary()
{
  G__Sizep2memfuncDictionary a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritanceDictionary() {

   /* Setting up class inheritance */
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetableDictionary() {

   /* Setting up typedef entry */
   G__search_typename2("Version_t",115,-1,0,-1);
   G__setnewtype(-1,"Class version identifier (short)",0);
   G__search_typename2("vector<TSchemaHelper>",117,G__get_linked_tagnum(&G__DictionaryLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__DictionaryLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__DictionaryLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__DictionaryLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__DictionaryLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<ROOT::TSchemaHelper>",117,G__get_linked_tagnum(&G__DictionaryLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<std::bidirectional_iterator_tag,TObject*,std::ptrdiff_t,const TObject**,const TObject*&>",117,G__get_linked_tagnum(&G__DictionaryLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<bidirectional_iterator_tag,TObject*>",117,G__get_linked_tagnum(&G__DictionaryLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<bidirectional_iterator_tag,TObject*,long>",117,G__get_linked_tagnum(&G__DictionaryLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<bidirectional_iterator_tag,TObject*,long,const TObject**>",117,G__get_linked_tagnum(&G__DictionaryLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */

   /* NGammaEvent */
static void G__setup_memvarNGammaEvent(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DictionaryLN_NGammaEvent));
   { NGammaEvent *p; p=(NGammaEvent*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->channel)-(long)(p)),105,0,0,-1,-1,-1,1,"channel=",0,"< channel on which event occured");
   G__memvar_setup((void*)((long)(&p->evtnum)-(long)(p)),110,0,0,-1,G__defined_typename("Long64_t"),-1,1,"evtnum=",0,"< sequential event number in run");
   G__memvar_setup((void*)((long)(&p->triggerTime)-(long)(p)),110,0,0,-1,G__defined_typename("Long64_t"),-1,1,"triggerTime=",0,"< time of event trigger since run start [ns]");
   G__memvar_setup((void*)((long)(&p->peakTime)-(long)(p)),110,0,0,-1,G__defined_typename("Long64_t"),-1,1,"peakTime=",0,"< time of event peak since run start [ns]");
   G__memvar_setup((void*)((long)(&p->centerDelta)-(long)(p)),100,0,0,-1,-1,-1,1,"centerDelta=",0,"< interpolated tweak to peak center timing");
   G__memvar_setup((void*)((long)(&p->peakTimeInterp)-(long)(p)),100,0,0,-1,-1,-1,1,"peakTimeInterp=",0,"< peak time interpolated");
   G__memvar_setup((void*)((long)(&p->pedestal)-(long)(p)),100,0,0,-1,-1,-1,1,"pedestal=",0,"< pedestal value (based on first 4 samples)");
   G__memvar_setup((void*)((long)(&p->minSample)-(long)(p)),105,0,0,-1,-1,-1,1,"minSample=",0,"< minimum sample value");
   G__memvar_setup((void*)((long)(&p->maxSample)-(long)(p)),105,0,0,-1,-1,-1,1,"maxSample=",0,"< maximum sample value");
   G__memvar_setup((void*)((long)(&p->maxInterp)-(long)(p)),100,0,0,-1,-1,-1,1,"maxInterp=",0,"< maximum value interpolated from peak samples");
   G__memvar_setup((void*)((long)(&p->area)-(long)(p)),100,0,0,-1,-1,-1,1,"area=",0,"< integrated area");
   G__memvar_setup((void*)((long)(&p->t1)-(long)(p)),100,0,0,-1,-1,-1,1,"t1=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->dt)-(long)(p)),100,0,0,-1,-1,-1,1,"dt=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->p1)-(long)(p)),100,0,0,-1,-1,-1,1,"p1=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->p2)-(long)(p)),100,0,0,-1,-1,-1,1,"p2=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->dEdx)-(long)(p)),100,0,0,-1,-1,-1,1,"dEdx=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->dEdx_tail)-(long)(p)),100,0,0,-1,-1,-1,1,"dEdx_tail=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->dEdx_min)-(long)(p)),100,0,0,-1,-1,-1,1,"dEdx_min=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->chi2)-(long)(p)),100,0,0,-1,-1,-1,1,"chi2=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__DictionaryLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   G__memvar_setup((void*)0,108,0,0,-1,-1,-1,4,"G__virtualinfo=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}

extern "C" void G__cpp_setup_memvarDictionary() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/
static void G__setup_memfuncNGammaEvent(void) {
   /* NGammaEvent */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DictionaryLN_NGammaEvent));
   G__memfunc_setup("Class",502,G__Dictionary_189_0_1, 85, G__get_linked_tagnum(&G__DictionaryLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) (TClass* (*)())(&NGammaEvent::Class), 0);
   G__memfunc_setup("Class_Name",982,G__Dictionary_189_0_2, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) (const char* (*)())(&NGammaEvent::Class_Name), 0);
   G__memfunc_setup("Class_Version",1339,G__Dictionary_189_0_3, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) (Version_t (*)())(&NGammaEvent::Class_Version), 0);
   G__memfunc_setup("Dictionary",1046,G__Dictionary_189_0_4, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) (void (*)())(&NGammaEvent::Dictionary), 0);
   G__memfunc_setup("IsA",253,G__Dictionary_189_0_5, 85, G__get_linked_tagnum(&G__DictionaryLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,G__Dictionary_189_0_6, 121, -1, -1, 0, 2, 1, 1, 0, 
"u 'TMemberInspector' - 1 - insp C - - 0 - parent", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,G__Dictionary_189_0_7, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - b", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__Dictionary_189_0_8, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__Dictionary_189_0_9, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) (const char* (*)())(&NGammaEvent::DeclFileName), 0);
   G__memfunc_setup("ImplFileLine",1178,G__Dictionary_189_0_10, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) (int (*)())(&NGammaEvent::ImplFileLine), 0);
   G__memfunc_setup("ImplFileName",1171,G__Dictionary_189_0_11, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) (const char* (*)())(&NGammaEvent::ImplFileName), 0);
   G__memfunc_setup("DeclFileLine",1152,G__Dictionary_189_0_12, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) (int (*)())(&NGammaEvent::DeclFileLine), 0);
   // automatic default constructor
   G__memfunc_setup("NGammaEvent", 1075, G__Dictionary_189_0_13, (int) ('i'), G__get_linked_tagnum(&G__DictionaryLN_NGammaEvent), -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic copy constructor
   G__memfunc_setup("NGammaEvent", 1075, G__Dictionary_189_0_14, (int) ('i'), G__get_linked_tagnum(&G__DictionaryLN_NGammaEvent), -1, 0, 1, 1, 1, 0, "u 'NGammaEvent' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~NGammaEvent", 1201, G__Dictionary_189_0_15, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__Dictionary_189_0_16, (int) ('u'), G__get_linked_tagnum(&G__DictionaryLN_NGammaEvent), -1, 1, 1, 1, 1, 0, "u 'NGammaEvent' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}


/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncDictionary() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();

}

static void G__cpp_setup_global1() {

   G__resetglobalenv();
}
extern "C" void G__cpp_setup_globalDictionary() {
  G__cpp_setup_global0();
  G__cpp_setup_global1();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

}

static void G__cpp_setup_func1() {
}

static void G__cpp_setup_func2() {
}

static void G__cpp_setup_func3() {
}

static void G__cpp_setup_func4() {
}

static void G__cpp_setup_func5() {
}

static void G__cpp_setup_func6() {
}

static void G__cpp_setup_func7() {
}

static void G__cpp_setup_func8() {
}

static void G__cpp_setup_func9() {
}

static void G__cpp_setup_func10() {
}

static void G__cpp_setup_func11() {

   G__resetifuncposition();
}

extern "C" void G__cpp_setup_funcDictionary() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
  G__cpp_setup_func2();
  G__cpp_setup_func3();
  G__cpp_setup_func4();
  G__cpp_setup_func5();
  G__cpp_setup_func6();
  G__cpp_setup_func7();
  G__cpp_setup_func8();
  G__cpp_setup_func9();
  G__cpp_setup_func10();
  G__cpp_setup_func11();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__DictionaryLN_TClass = { "TClass" , 99 , -1 };
G__linked_taginfo G__DictionaryLN_TBuffer = { "TBuffer" , 99 , -1 };
G__linked_taginfo G__DictionaryLN_TMemberInspector = { "TMemberInspector" , 99 , -1 };
G__linked_taginfo G__DictionaryLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR = { "vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >" , 99 , -1 };
G__linked_taginfo G__DictionaryLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR = { "reverse_iterator<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::iterator>" , 99 , -1 };
G__linked_taginfo G__DictionaryLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR = { "iterator<bidirectional_iterator_tag,TObject*,long,const TObject**,const TObject*&>" , 115 , -1 };
G__linked_taginfo G__DictionaryLN_NGammaEvent = { "NGammaEvent" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtableDictionary() {
  G__DictionaryLN_TClass.tagnum = -1 ;
  G__DictionaryLN_TBuffer.tagnum = -1 ;
  G__DictionaryLN_TMemberInspector.tagnum = -1 ;
  G__DictionaryLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR.tagnum = -1 ;
  G__DictionaryLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__DictionaryLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR.tagnum = -1 ;
  G__DictionaryLN_NGammaEvent.tagnum = -1 ;
}


extern "C" void G__cpp_setup_tagtableDictionary() {

   /* Setting up class,struct,union tag entry */
   G__get_linked_tagnum_fwd(&G__DictionaryLN_TClass);
   G__get_linked_tagnum_fwd(&G__DictionaryLN_TBuffer);
   G__get_linked_tagnum_fwd(&G__DictionaryLN_TMemberInspector);
   G__get_linked_tagnum_fwd(&G__DictionaryLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR);
   G__get_linked_tagnum_fwd(&G__DictionaryLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__DictionaryLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR);
   G__tagtable_setup(G__get_linked_tagnum(&G__DictionaryLN_NGammaEvent),sizeof(NGammaEvent),-1,0,"Neutron-gamma event",G__setup_memvarNGammaEvent,G__setup_memfuncNGammaEvent);
}
extern "C" void G__cpp_setupDictionary(void) {
  G__check_setup_version(30051515,"G__cpp_setupDictionary()");
  G__set_cpp_environmentDictionary();
  G__cpp_setup_tagtableDictionary();

  G__cpp_setup_inheritanceDictionary();

  G__cpp_setup_typetableDictionary();

  G__cpp_setup_memvarDictionary();

  G__cpp_setup_memfuncDictionary();
  G__cpp_setup_globalDictionary();
  G__cpp_setup_funcDictionary();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncDictionary();
  return;
}
class G__cpp_setup_initDictionary {
  public:
    G__cpp_setup_initDictionary() { G__add_setup_func("Dictionary",(G__incsetup)(&G__cpp_setupDictionary)); G__call_setup_funcs(); }
   ~G__cpp_setup_initDictionary() { G__remove_setup_func("Dictionary"); }
};
G__cpp_setup_initDictionary G__cpp_setup_initializerDictionary;

