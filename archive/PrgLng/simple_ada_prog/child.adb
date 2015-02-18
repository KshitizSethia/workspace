
with Text_Io;
use Text_Io;

package body Child is

   package Int_Io is new Integer_Io(Integer);
   use Int_Io;

   procedure SetY(Obj: out B; Val: Integer) is
   begin
      Obj.Y := Val;
   end;

   function GetY(Obj: B) return Integer is
   begin
      return Obj.Y;
   end;

   procedure PutFields(Obj: B) is
   begin
      Put("Object type is B: X = ");
      Put(Obj.X);
      Put(", Y = ");
      Put(Obj.Y);
      New_line;
   end;

end Child;

