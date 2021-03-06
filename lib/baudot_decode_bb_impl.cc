/* -*- c++ -*- */
/* 
 * Copyright 2017 Phil Frost.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "baudot_decode_bb_impl.h"

namespace gr {
  namespace radioteletype {
    baudot_decode_bb::sptr baudot_decode_bb::make()
    {
      return gnuradio::get_initial_sptr
        (new baudot_decode_bb_impl());
    }

    baudot_decode_bb_impl::baudot_decode_bb_impl()
      : gr::block("baudot_decode_bb",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(char)))
    {
      char_set = letters;
    }

    baudot_decode_bb_impl::~baudot_decode_bb_impl()
    {}

    void baudot_decode_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }

    int baudot_decode_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const char *in = (const char *) input_items[0];
      char *out = (char *) output_items[0];

      const char *const in_start = in;
      const char *const out_start = out;

      while( (out - out_start < noutput_items) &&
             (in - in_start < ninput_items[0]))
      {
        if (*in == 0x1b)
        {
          char_set = figures;
        }
        else if (*in == 0x1f)
        {
          char_set = letters;
        }
        else
        {
          *out++ = char_set[*in & 0x1f];
        }
        in += 1;
      }

      consume_each (in - in_start);
      return out - out_start;
    }

  } /* namespace radioteletype */
} /* namespace gr */

