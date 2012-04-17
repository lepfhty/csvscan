require 'csvscan/csvscan'
require 'csv'

module CSVScan

  class Row
    attr_reader :headers
    def initialize(headers, array)
      @headers = headers
      @array = array
    end

    def [](idx_hdr, *args)
      if idx_hdr.is_a? String or idx_hdr.is_a? Symbol
        @array[@headers.index(idx_hdr.to_s)]
      else
        @array.send(:[], idx_hdr, *args)
      end
    end

    def []=(idx_hdr, val, *args)
      if idx_hdr.is_a? String or idx_hdr.is_a? Symbol
        @array[@headers.index(idx_hdr.to_s)] = val
      else
        @array.send(:[]=, idx_hdr, val, *args)
      end
    end

    def to_s
      @array.to_s
    end

    def method_missing(msg, *args, &blk)
      @array.send(msg, *args, &blk)
    end
  end

  def CSVScan.foreach(file, opts = {}, &blk)
    hdr = nil
    CSVScan.scan(File.new(file,'r')) do |row|
      if hdr.nil?
        hdr = Array.new(row)
        next if opts[:skip_header]
      end
      yield CSVScan::Row.new(hdr, row)
    end
  end

  def CSVScan.sort_rows(infile, outfile, headers)
    rows = []
    CSVScan.foreach(infile, :skip_header => false) do |row|
      rows << row
    end
    newh = rows.shift

    rows.sort_by! do |row|
      if headers.is_a? Array
        headers.map { |h| row[h] }
      elsif headers.is_a? Hash
        headers.map { |h,p| p.call(row[h]) }
      end
    end

    CSV.open(outfile, 'w') do |f|
      f << newh
      rows.each do |r|
        f << r
      end
    end
  end

  def CSVScan.filter_cols(infile, outfile, headers)
    CSV.open(outfile, 'w') do |out|
      hdone = false
      CSVScan.foreach(infile, :skip_header => false) do |row|
        if !hdone and headers.is_a? Hash
          out << headers.map { |h,newh| newh }
          headers = headers.map { |h,newh| h }
          hdone = true
        else
          out << headers.map { |h| row[h] }
        end
      end
    end
  end
end
