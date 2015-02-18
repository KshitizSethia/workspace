Public Class Form1

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        Dim warper As New Warper
        warper.setSource()

        warper.setDestination()

        warper.computeWarp()

        warper.warp()
    End Sub
End Class
