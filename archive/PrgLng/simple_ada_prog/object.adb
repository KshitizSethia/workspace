
with Text_Io;
use Text_Io;

package body Object is

   package Int_Io is new Integer_Io(Integer);
   use Int_Io;

   procedure SetX(Obj: out A; Val: Integer) is
   begin
      Obj.X := Val;
   end;

   function GetX(Obj: A) return Integer is
   begin
      return Obj.X;
   end;

   procedure PutFields(Obj: A) is
   begin
      Put("Object type is A: X = ");
      Put(Obj.X);
      New_line;
   end;

end Object;

