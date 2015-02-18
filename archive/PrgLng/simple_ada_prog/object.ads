
package Object is

   type A is tagged
      record
         X: Integer;
      end record;
   procedure SetX(Obj: out A; Val: Integer);
   function GetX(Obj: A) return Integer;
   procedure PutFields(Obj: A);
end Object;
