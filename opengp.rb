require 'formula'

# this formula installs the header only library!
class Opengp < Formula
  url 'https://github.com/OpenGP/OpenGP.git', :using => :git
  homepage 'http://opengp.github.io'
  version "1.0"

  depends_on "eigen"

  def install
    include.install Dir["src/*"]
  end
end