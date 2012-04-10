
#line 1 "ext/csvscan/csvscan.rl"
#include <ruby.h>

static VALUE rb_eCSVParseError;
static ID s_read, s_to_str;


#line 70 "ext/csvscan/csvscan.rl"



#line 14 "ext/csvscan/csvscan.c"
static const int csv_scan_start = 2;
static const int csv_scan_error = 0;

static const int csv_scan_en_main = 2;


#line 73 "ext/csvscan/csvscan.rl"

#define BUFSIZE 131072

VALUE csv_scan(VALUE self, VALUE port) {
    int cs, act, curline = 1;
    size_t have = 0, nread = 0;
    unsigned char *ts = NULL, *te = NULL, *buf;
    VALUE row, coldata;
    VALUE bufsize = Qnil;
    int done=0, buffer_size;

    if ( !rb_respond_to( port, s_read ) ) {
        if ( rb_respond_to( port, s_to_str ) ) {
            port = rb_funcall( port, s_to_str, 0 );
            StringValue(port);
        } else {
            rb_raise( rb_eArgError, "bad argument, String or IO only please." );
        }
    }

    buffer_size = BUFSIZE;
    if (rb_ivar_defined(self, rb_intern("@buffer_size")) == Qtrue) {
        bufsize = rb_ivar_get(self, rb_intern("@buffer_size"));
        if (!NIL_P(bufsize)) {
            buffer_size = NUM2INT(bufsize);
        }
    }
    buf = ALLOC_N(unsigned char, buffer_size);

    
#line 52 "ext/csvscan/csvscan.c"
	{
	cs = csv_scan_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 103 "ext/csvscan/csvscan.rl"

    row = rb_ary_new();
    coldata = Qnil;

    while( !done ) {
        VALUE str;
        unsigned char *p = buf + have, *pe, *eof = 0;
        size_t len, space = buffer_size - have;

        if ( space == 0 ) {
            rb_raise(rb_eCSVParseError, "ran out of buffer on line %d.", curline);
        }

        if ( rb_respond_to( port, s_read ) ) {
            str = rb_funcall( port, s_read, 1, INT2FIX(space) );
        } else {
            str = rb_str_substr( port, nread, space );
        }

        StringValue(str);
        memcpy( p, RSTRING_PTR(str), RSTRING_LEN(str) );
        len = RSTRING_LEN(str);
        nread += len;

        /* If this is the last buffer, set EOF to the end of data. */
        if ( len < space ) {
            eof = pe;
            done = 1;
        }

        pe = p + len;
        
#line 93 "ext/csvscan/csvscan.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr0:
#line 1 "NONE"
	{	switch( act ) {
	case 0:
	{{goto st0;}}
	break;
	case 4:
	{{p = ((te))-1;}
          unsigned char ch, *endp;
          size_t datalen;
          datalen = te - ts;
          endp = te - 1;
          while(datalen>0) {
              ch = *endp--;
              if (ch==' ' || ch=='\t') {
                  datalen--;
              } else {
                  break;
              }
          }
          if (datalen==0) {
              coldata = Qnil;
          } else {
              coldata = rb_str_new(ts, datalen);
          }
      }
	break;
	case 5:
	{{p = ((te))-1;}
          unsigned char ch, *start_p, *wptr, *rptr;
          size_t rest, datalen;
          start_p = wptr = ts;
          rptr = ts + 1;
          rest = te - ts - 2;
          datalen = 0;
          while(rest>0) {
              ch = *rptr++;
              if (ch=='"') {
                rptr++;
                rest--;
              }
              *wptr++ = ch;
              datalen++;
              rest--;
          }
          coldata = rb_str_new( start_p, datalen );
      }
	break;
	default:
	{{p = ((te))-1;}}
	break;
	}
	}
	goto st2;
tr5:
#line 19 "ext/csvscan/csvscan.rl"
	{te = p+1;}
	goto st2;
tr6:
#line 19 "ext/csvscan/csvscan.rl"
	{te = p+1;}
#line 10 "ext/csvscan/csvscan.rl"
	{
        curline += 1;
    }
#line 20 "ext/csvscan/csvscan.rl"
	{
          rb_ary_push(row, coldata);
          rb_yield(row);
          coldata = Qnil;
          row = rb_ary_new();
      }
	goto st2;
tr9:
#line 26 "ext/csvscan/csvscan.rl"
	{te = p+1;{
          rb_ary_push(row, coldata);
          coldata = Qnil;
      }}
	goto st2;
tr10:
#line 19 "ext/csvscan/csvscan.rl"
	{te = p;p--;}
	goto st2;
tr11:
#line 10 "ext/csvscan/csvscan.rl"
	{
        curline += 1;
    }
#line 20 "ext/csvscan/csvscan.rl"
	{
          rb_ary_push(row, coldata);
          rb_yield(row);
          coldata = Qnil;
          row = rb_ary_new();
      }
#line 25 "ext/csvscan/csvscan.rl"
	{te = p+1;}
	goto st2;
tr12:
#line 49 "ext/csvscan/csvscan.rl"
	{te = p;p--;{
          unsigned char ch, *start_p, *wptr, *rptr;
          size_t rest, datalen;
          start_p = wptr = ts;
          rptr = ts + 1;
          rest = te - ts - 2;
          datalen = 0;
          while(rest>0) {
              ch = *rptr++;
              if (ch=='"') {
                rptr++;
                rest--;
              }
              *wptr++ = ch;
              datalen++;
              rest--;
          }
          coldata = rb_str_new( start_p, datalen );
      }}
	goto st2;
st2:
#line 1 "NONE"
	{ts = 0;}
#line 1 "NONE"
	{act = 0;}
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1 "NONE"
	{ts = p;}
#line 230 "ext/csvscan/csvscan.c"
	switch( (*p) ) {
		case 9u: goto tr5;
		case 10u: goto tr6;
		case 13u: goto st4;
		case 32u: goto tr5;
		case 34u: goto st1;
		case 44u: goto tr9;
	}
	if ( 11u <= (*p) && (*p) <= 12u )
		goto tr7;
	goto tr4;
tr4:
#line 1 "NONE"
	{te = p+1;}
#line 30 "ext/csvscan/csvscan.rl"
	{act = 4;}
	goto st3;
tr7:
#line 1 "NONE"
	{te = p+1;}
#line 19 "ext/csvscan/csvscan.rl"
	{act = 1;}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 258 "ext/csvscan/csvscan.c"
	switch( (*p) ) {
		case 10u: goto tr0;
		case 13u: goto tr0;
		case 34u: goto tr0;
		case 44u: goto tr0;
	}
	goto tr4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 10u )
		goto tr11;
	goto tr10;
tr2:
#line 10 "ext/csvscan/csvscan.rl"
	{
        curline += 1;
    }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 283 "ext/csvscan/csvscan.c"
	switch( (*p) ) {
		case 10u: goto tr2;
		case 34u: goto tr3;
	}
	goto st1;
tr3:
#line 1 "NONE"
	{te = p+1;}
#line 49 "ext/csvscan/csvscan.rl"
	{act = 5;}
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 299 "ext/csvscan/csvscan.c"
	if ( (*p) == 34u )
		goto st1;
	goto tr12;
st0:
cs = 0;
	goto _out;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 3: goto tr0;
	case 4: goto tr10;
	case 1: goto tr0;
	case 5: goto tr12;
	}
	}

	_out: {}
	}

#line 135 "ext/csvscan/csvscan.rl"

        if ( cs == csv_scan_error ) {
            free(buf);
            rb_raise(rb_eCSVParseError, "parse error on line %d.", curline);
        }

        if ( ts == 0 ) {
            have = 0;
        } else {
            have = pe - ts;
            memmove( buf, ts, have );
            te = buf + (te - ts);
            ts = buf;
        }
    }
    free(buf);
    return Qnil;
}

void Init_csvscan() {
  VALUE mCSVScan = rb_define_module("CSVScan");
  rb_define_attr(rb_singleton_class(mCSVScan), "buffer_size", 1, 1);
  rb_define_singleton_method(mCSVScan, "scan", csv_scan, 1);
  rb_eCSVParseError = rb_define_class_under(mCSVScan, "ParseError", rb_eException);

  s_read = rb_intern("read");
  s_to_str = rb_intern("to_str");
}
