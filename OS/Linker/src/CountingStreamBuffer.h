/*
 * CountingStreamBuffer.h
 *	wraps file io to give line and column number
 *  Created on: Feb 8, 2015
 *  Copied from: http://stackoverflow.com/questions/4813129/how-to-get-the-line-number-from-a-file-in-c
 */

#ifndef COUNTINGSTREAMBUFFER_H_
#define COUNTINGSTREAMBUFFER_H_

#include <streambuf>

class CountingStreamBuffer : public std::streambuf
{
public:
    // constructor
    CountingStreamBuffer(std::streambuf* sbuf) :
        streamBuf_(sbuf),
        lineNumber_(1),
        lastLineNumber_(1),
        column_(0),
        prevColumn_(static_cast<unsigned int>(-1)),
        filePos_(0)
    {
    }

    // Get current line number
    unsigned int        lineNumber() const  { return lineNumber_; }

    // Get line number of previously read character
    unsigned int        prevLineNumber() const { return lastLineNumber_; }

    // Get current column
    unsigned int        column() const   { return column_; }

    // Get previous column
    unsigned int prev_column() const   { return prevColumn_; }

    // Get file position
    std::streamsize     filepos() const { return filePos_; }

    //CountingStreamBuffer& operator=(const CountingStreamBuffer& other)
    //{
    //	CountingStreamBuffer result(other);
    //	return result;
    //}
    CountingStreamBuffer(const CountingStreamBuffer& other)
    {
    	this->streamBuf_ 		= other.streamBuf_     ;
    	this->lineNumber_		= other.lineNumber_    ;
		this->lastLineNumber_   = other.lastLineNumber_;
		this->column_           = other.column_        ;
		this->prevColumn_       = other.prevColumn_    ;
		this->filePos_          = other.filePos_       ;
    }

protected:

    // extract next character from stream w/o advancing read pos
    std::streambuf::int_type underflow()
    {
        return streamBuf_->sgetc();
    }

    // extract next character from stream
    std::streambuf::int_type uflow()
    {
        int_type rc = streamBuf_->sbumpc();

        lastLineNumber_ = lineNumber_;
        if (traits_type::eq_int_type(rc, traits_type::to_int_type('\n')))
        {
            ++lineNumber_;
            prevColumn_ = column_ + 1;
            column_ = static_cast<unsigned int>(-1);
        }

        ++column_;
        ++filePos_;
        return rc;
    }

    // put back last character
    std::streambuf::int_type pbackfail(std::streambuf::int_type c)
    {
        if (traits_type::eq_int_type(c, traits_type::to_int_type('\n')))
        {
            --lineNumber_;
            lastLineNumber_ = lineNumber_;
            column_ = prevColumn_;
            prevColumn_ = 0;
        }

        --column_;
        --filePos_;

        if (c != traits_type::eof())
            return streamBuf_->sputbackc(traits_type::to_char_type(c));
        else
            return streamBuf_->sungetc();
    }

    // change position by offset, according to way and mode
    virtual std::ios::pos_type seekoff(std::ios::off_type pos,
                                  std::ios_base::seekdir dir,
                                  std::ios_base::openmode mode)
    {
        if (dir == std::ios_base::beg
         && pos == static_cast<std::ios::off_type>(0))
        {
            lastLineNumber_ = 1;
            lineNumber_ = 1;
            column_ = 0;
            prevColumn_ = static_cast<unsigned int>(-1);
            filePos_ = 0;

            return streamBuf_->pubseekoff(pos, dir, mode);
        }
        else
            return std::streambuf::seekoff(pos, dir, mode);
    }

    // change to specified position, according to mode
    virtual std::ios::pos_type seekpos(std::ios::pos_type pos,
                                  std::ios_base::openmode mode)
    {
        if (pos == static_cast<std::ios::pos_type>(0))
        {
            lastLineNumber_ = 1;
            lineNumber_ = 1;
            column_ = 0;
            prevColumn_ = static_cast<unsigned int>(-1);
            filePos_ = 0;

            return streamBuf_->pubseekpos(pos, mode);
        }
        else
            return std::streambuf::seekpos(pos, mode);
    }


private:
    std::streambuf*     streamBuf_;     // hosted streambuffer
    unsigned int        lineNumber_;    // current line number
    unsigned int        lastLineNumber_;// line number of last read character
    unsigned int        column_;        // current column
    unsigned int        prevColumn_;    // previous column
    std::streamsize     filePos_;       // file position
};

#endif /* COUNTINGSTREAMBUFFER_H_ */
