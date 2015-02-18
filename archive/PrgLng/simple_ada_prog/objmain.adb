
with Text_Io;
use Text_Io;

with Object;
use Object;

with Child;
use Child;

procedure ObjMain is
   package Int_Io is new Integer_Io(Integer);
   use Int_Io;

   Obj: A;
   Obj2: B;

   procedure Foo(AnyObj: in out A'Class) is
   begin
      SetX(AnyObj, 22);
      PutFields(AnyObj);
   end;

begin
   SetX(Obj,55);
   PutFields(Obj); New_Line;
   Foo(Obj);
   PutFields(Obj); New_Line;
   Put("-----------------"); New_Line;
   SetX(Obj2,11);
   SetY(Obj2,12);
   PutFields(Obj2); New_Line;
   Foo(Obj2);
   PutFields(Obj2); New_Line;

end ObjMain;

