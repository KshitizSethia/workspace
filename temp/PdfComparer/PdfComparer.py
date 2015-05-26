import os
import sys
from subprocess import PIPE, Popen
import glob

#from string.Formatter import format


num_digits_for_pages_in_pdf = 2
fuzz = "20%"


def main():
    if(len(sys.argv) < 4):
        print "Usage: PdfComparer pdf_baseline pdf_new results_folder [job_name]"
        print "Output: HTML table containing results"
        sys.exit()

    ## read input params and get absolute paths from them     
    pdf_base = os.path.realpath(sys.argv[1])
    pdf_new = os.path.realpath(sys.argv[2])
    result_folder = os.path.realpath(sys.argv[3])
    
    
    ## name for this comparison
    jobName = ""
    if(len(sys.argv)==5):
        jobName = sys.argv[4]
    else:
        jobName = os.path.splitext(os.path.basename(pdf_base))[0]

    ## prepare error message
    error_in_job = "<table border=\"1\"> <tr bgcolor=\"E67373\"> <td>Error in job: %s</td> </tr> </table>" % jobName

    ## create output folder if it does not exist
    if not os.path.exists(result_folder):os.makedirs(result_folder)
    
    ## baseline pdf to images
    ##
    path_base = "%s\%s-base-" %(result_folder, jobName)
    convert_cmd_base = "convert -density 600 \"%s\" \"%s%%0%dd.jpg\"" %(pdf_base, path_base, num_digits_for_pages_in_pdf)
    os.popen(convert_cmd_base)
    #dummy, error = Popen(convert_cmd_base, stderr=PIPE).communicate()
    #if(len(error)>0):
    #    sys.stderr.write(error)
    #    print error_in_job
    #    sys.exit()
    
    ## new pdf to images
    ##
    path_new = "%s\%s-new-" % (result_folder, jobName)
    convert_cmd_new = "convert -density 600 \"%s\" \"%s%%0%dd.jpg\"" % (pdf_new, path_new, num_digits_for_pages_in_pdf)
    os.popen(convert_cmd_new)
    #dummy, error = Popen(convert_cmd_new, stderr=PIPE).communicate()
    #if(len(error)>0):
    #    sys.stderr.write(error)
    #    print error_in_job
    #    sys.exit()
    
    ## Compare both pdfs
    allSame = True
    similarity = []
    img_base = glob.glob(path_base+"*")
    img_new = glob.glob(path_new+"*")
    img_diff = []
    
    for counter in range(len(img_base)):
        
        filename_diff = "%s-diff-%02d.jpg" % (jobName, counter)
        filepath_diff = os.path.join(result_folder, filename_diff)
        img_diff.append(filepath_diff)
        compare_command = "compare -metric AE -fuzz \"%s\" \"%s\" \"%s\" \"%s\"" %(fuzz, img_base[counter],
                                                                                    img_new[counter], filepath_diff)
        dummy, pixel_diff = Popen(compare_command, stderr=PIPE).communicate()
        if(int(pixel_diff) > 0):
            #print "Not Same: %s" % (filename)
            similarity.append(False)
            allSame=False
        else:
        #    print "Same: %s" % (filename)
            similarity.append(True)
            
    ## make detailed report with all pages
    html_detail = ["<table border=\"1\"> <col width=\"1%\"> <col width=\"33%\"> <col width=\"33%\"> <col width=\"33%\"> <tr> <th>Page</th> <th>Baseline</th> <th>Test</th> <th>Difference</th> </tr>"]
    for counter in range(len(img_base)):
        if(similarity[counter]==True):
            html_detail.append(" <tr bgcolor=\"CCFF33\"> <td>%d</td> <td><img width=\"100%%\" height=\"auto\" alt=\"\" src=\"%s\"></td> <td><img width=\"100%%\" height=\"auto\" alt=\"\" src=\"%s\"></td> <td><img width=\"100%%\" height=\"auto\" alt=\"\" src=\"%s\"></td> </tr>"
                               % (counter+1, img_base[counter], img_new[counter], img_diff[counter]))
        else:
            html_detail.append(" <tr bgcolor=\"E67373\"> <td>%d</td> <td><img width=\"100%%\" height=\"auto\" alt=\"\" src=\"%s\"></td> <td><img width=\"100%%\" height=\"auto\" alt=\"\" src=\"%s\"></td> <td><img width=\"100%%\" height=\"auto\" alt=\"\" src=\"%s\"></td> </tr>"
                               % (counter+1, img_base[counter], img_new[counter], img_diff[counter]))
    html_detail.append("</table>")
    
    html_detail_filename = os.path.join(result_folder, "%s-details.htm" % jobName)
    file_detail = open(html_detail_filename, "w")
    file_detail.write(''.join(html_detail))
    file_detail.close()
    
    ## output a row with summary
    html_summary = ["<table border=\"1\">"]# <tr> <th>Job Name</th> <th>Baseline</th> <th>Test</th> <th>Result</th> </tr>"]
    if(allSame):
        html_summary.append("<tr bgcolor=\"CCFF33\"> <td>%s</td> <td><a href=\"%s\">%s</a></td> <td><a href=\"%s\">%s</a></td> <td><a href=\"%s\">%s</a></td> </tr>"
                            % (jobName, pdf_base, os.path.basename(pdf_base), pdf_new, os.path.basename(pdf_new), html_detail_filename, "Same"))
    else:
        html_summary.append("<tr bgcolor=\"E67373\"> <td>%s</td> <td><a href=\"%s\">%s</a></td> <td><a href=\"%s\">%s</a></td> <td><a href=\"%s\">%s</a></td> </tr>"
                            % (jobName, pdf_base, os.path.basename(pdf_base), pdf_new, os.path.basename(pdf_new), html_detail_filename, "Not Same"))
    html_summary.append("</table>")
    print "".join(html_summary)
    
if __name__ == "__main__":
    main()
