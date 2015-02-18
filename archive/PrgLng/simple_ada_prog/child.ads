
with Object;
use Object;

package Child is

   type B is new A with
      record
         Y: Integer;
      end record;
--   function NewB return B;
   procedure SetY(Obj: out B; Val: Integer);
   function GetY(Obj: B) return Integer;
   procedure PutFields(Obj: B);
end Child;
