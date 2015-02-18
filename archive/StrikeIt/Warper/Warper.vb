Class Warper
    Private srcX As Single() = New Single(3) {}
    Private srcY As Single() = New Single(3) {}
    Private dstX As Single() = New Single(3) {}
    Private dstY As Single() = New Single(3) {}
    Private srcMat As Single() = New Single(15) {}
    Private dstMat As Single() = New Single(15) {}
    Private warpMat As Single() = New Single(15) {}
    Private dirty As Boolean

    Public Sub New()
        setIdentity()
    End Sub

    Public Sub setIdentity()
        setSource(0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, _
         1.0F, 1.0F)
        setDestination(0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, _
         1.0F, 1.0F)
        computeWarp()
    End Sub

    Public Sub setSource(ByVal x0 As Single, ByVal y0 As Single, ByVal x1 As Single, ByVal y1 As Single, ByVal x2 As Single, ByVal y2 As Single, _
     ByVal x3 As Single, ByVal y3 As Single)
        srcX(0) = x0
        srcY(0) = y0
        srcX(1) = x1
        srcY(1) = y1
        srcX(2) = x2
        srcY(2) = y2
        srcX(3) = x3
        srcY(3) = y3
        dirty = True
    End Sub

    Public Sub setDestination(ByVal x0 As Single, ByVal y0 As Single, ByVal x1 As Single, ByVal y1 As Single, ByVal x2 As Single, ByVal y2 As Single, _
     ByVal x3 As Single, ByVal y3 As Single)
        dstX(0) = x0
        dstY(0) = y0
        dstX(1) = x1
        dstY(1) = y1
        dstX(2) = x2
        dstY(2) = y2
        dstX(3) = x3
        dstY(3) = y3
        dirty = True
    End Sub


    Public Sub computeWarp()
        computeQuadToSquare(srcX(0), srcY(0), srcX(1), srcY(1), srcX(2), srcY(2), _
         srcX(3), srcY(3), srcMat)
        computeSquareToQuad(dstX(0), dstY(0), dstX(1), dstY(1), dstX(2), dstY(2), _
         dstX(3), dstY(3), dstMat)
        multMats(srcMat, dstMat, warpMat)
        dirty = False
    End Sub

    Public Sub multMats(ByVal srcMat As Single(), ByVal dstMat As Single(), ByVal resMat As Single())
        ' DSTDO/CBB: could be faster, but not called often enough to matter
        For r As Integer = 0 To 3
            Dim ri As Integer = r * 4
            For c As Integer = 0 To 3
                resMat(ri + c) = (srcMat(ri) * dstMat(c) + srcMat(ri + 1) * dstMat(c + 4) + srcMat(ri + 2) * dstMat(c + 8) + srcMat(ri + 3) * dstMat(c + 12))
            Next
        Next
    End Sub

    Public Sub computeSquareToQuad(ByVal x0 As Single, ByVal y0 As Single, ByVal x1 As Single, ByVal y1 As Single, ByVal x2 As Single, ByVal y2 As Single, _
     ByVal x3 As Single, ByVal y3 As Single, ByVal mat As Single())

        Dim dx1 As Single = x1 - x2, dy1 As Single = y1 - y2
        Dim dx2 As Single = x3 - x2, dy2 As Single = y3 - y2
        Dim sx As Single = x0 - x1 + x2 - x3
        Dim sy As Single = y0 - y1 + y2 - y3
        Dim g As Single = (sx * dy2 - dx2 * sy) / (dx1 * dy2 - dx2 * dy1)
        Dim h As Single = (dx1 * sy - sx * dy1) / (dx1 * dy2 - dx2 * dy1)
        Dim a As Single = x1 - x0 + g * x1
        Dim b As Single = x3 - x0 + h * x3
        Dim c As Single = x0
        Dim d As Single = y1 - y0 + g * y1
        Dim e As Single = y3 - y0 + h * y3
        Dim f As Single = y0

        mat(0) = a
        mat(1) = d
        mat(2) = 0
        mat(3) = g
        mat(4) = b
        mat(5) = e
        mat(6) = 0
        mat(7) = h
        mat(8) = 0
        mat(9) = 0
        mat(10) = 1
        mat(11) = 0
        mat(12) = c
        mat(13) = f
        mat(14) = 0
        mat(15) = 1
    End Sub

    Public Sub computeQuadToSquare(ByVal x0 As Single, ByVal y0 As Single, ByVal x1 As Single, ByVal y1 As Single, ByVal x2 As Single, ByVal y2 As Single, _
     ByVal x3 As Single, ByVal y3 As Single, ByVal mat As Single())
        computeSquareToQuad(x0, y0, x1, y1, x2, y2, _
         x3, y3, mat)

        ' invert through adjoint

        ' ignore 
        Dim a__1 As Single = mat(0), d__2 As Single = mat(1), g__3 As Single = mat(3)
        ' 3rd col
        Dim b__4 As Single = mat(4), e__5 As Single = mat(5), h__6 As Single = mat(7)
        ' ignore 3rd row 

        Dim c__7 As Single = mat(12), f__8 As Single = mat(13)

        Dim A__9 As Single = e__5 - f__8 * h__6
        Dim B__10 As Single = c__7 * h__6 - b__4
        Dim C__11 As Single = b__4 * f__8 - c__7 * e__5
        Dim D__12 As Single = f__8 * g__3 - d__2
        Dim E__13 As Single = a__1 - c__7 * g__3
        Dim F__14 As Single = c__7 * d__2 - a__1 * f__8
        Dim G__15 As Single = d__2 * h__6 - e__5 * g__3
        Dim H__16 As Single = b__4 * g__3 - a__1 * h__6
        Dim I As Single = a__1 * e__5 - b__4 * d__2

        ' Probably unnecessary since 'I' is also scaled by the determinant,
        '   and 'I' scales the homogeneous coordinate, which, in turn,
        '   scales the X,Y coordinates.
        ' Determinant  =   a * (e - f * h) + b * (f * g - d) + c * (d * h - e * g);
        Dim idet As Single = 1.0F / (a__1 * A__9 + b__4 * D__12 + c__7 * G__15)

        mat(0) = A__9 * idet
        mat(1) = D__12 * idet
        mat(2) = 0
        mat(3) = G__15 * idet
        mat(4) = B__10 * idet
        mat(5) = E__13 * idet
        mat(6) = 0
        mat(7) = H__16 * idet
        mat(8) = 0
        mat(9) = 0
        mat(10) = 1
        mat(11) = 0
        mat(12) = C__11 * idet
        mat(13) = F__14 * idet
        mat(14) = 0
        mat(15) = I * idet
    End Sub

    Public Function getWarpMatrix() As Single()
        Return warpMat
    End Function

    Public Sub warp(ByVal srcX As Single, ByVal srcY As Single, ByRef dstX As Single, ByRef dstY As Single)
        If dirty Then
            computeWarp()
        End If
        Warper.warp(warpMat, srcX, srcY, dstX, dstY)
    End Sub

    Public Shared Sub warp(ByVal mat As Single(), ByVal srcX As Single, ByVal srcY As Single, ByRef dstX As Single, ByRef dstY As Single)
        Dim result As Single() = New Single(3) {}
        Dim z As Single = 0
        result(0) = CSng(srcX * mat(0) + srcY * mat(4) + z * mat(8) + 1 * mat(12))
        result(1) = CSng(srcX * mat(1) + srcY * mat(5) + z * mat(9) + 1 * mat(13))
        result(2) = CSng(srcX * mat(2) + srcY * mat(6) + z * mat(10) + 1 * mat(14))
        result(3) = CSng(srcX * mat(3) + srcY * mat(7) + z * mat(11) + 1 * mat(15))
        dstX = result(0) / result(3)
        dstY = result(1) / result(3)
    End Sub
End Class