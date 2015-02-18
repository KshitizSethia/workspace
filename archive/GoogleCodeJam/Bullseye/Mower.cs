using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Lawnmower
{
    static class Mower
    {
        internal static bool CanMowRowWithoutDestroyingPattern(int rowToMow, int desiredHeight, Lawn lawn)
        {
            bool result = true;
            for (int columnNumber = 0; columnNumber < lawn.lawnColumns; columnNumber++)
            {
                if (desiredHeight < lawn.parsedLawn[rowToMow, columnNumber])
                {
                    result = false;
                    break;
                }
            }

            return result;
        }

        internal static bool CanMowColumnWithoutDestroyingPattern(int columnToMow, int desiredHeight, Lawn lawn)
        {
            bool result = true;
            for (int rowNumber = 0; rowNumber < lawn.lawnRows; rowNumber++)
            {
                if (desiredHeight < lawn.parsedLawn[rowNumber, columnToMow])
                {
                    result = false;
                    break;
                }
            }

            return result;
        }
    }
}
