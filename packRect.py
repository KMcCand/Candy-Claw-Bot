from rectpack import newPacker
import random

if True:
  s = '''25.66771654 22.83464567
  26.35866142 22.83464567
  2.323622047 1.968503937
  14.15393701 1.968503937
  1.968503937 20.86614173
  19.68503937 26.37795276
  19.76220472 5.905511811
  26.35866142 5.905511811
  19.68503937 25.59055118
  20.94330709 14.19251969
  25.66771654 26.45511811
  14.21181102 20.94330709'''

  s2 = '''1
  2
  2
  2
  1
  1
  1
  2
  1
  1
  1
  1'''
else:
  s = '''24 24
  34 34
  14 34
  14 104
  25 69
  24 104
  24 49'''

  s2 = '''1
  1
  1
  1
  1
  1
  1'''
  
packer = newPacker()

quantities = s2.split('\n')

upto = 0
expectedRectCount = 0

for line in s.split('\n'):
  w, h = [float(x) for x in line.strip().split()]

  w = int(8 * w)/8
  h = int(8 * h)/8
  quantity = int(quantities[upto])
  upto += 1
  print('quant %s' % quantity)
  for i in range(quantity):
    packer.add_rect(w, h)
    expectedRectCount += 1
  print('add rect %s, %s' % (w, h))

if False:
  for i in range(400):
    w = random.randint(5, 40)
    h = random.randint(5, 40)
    packer.add_rect(w, h)

packer.add_bin(30, 32, 3)
packer.add_bin(24, 30, 4)
#packer.add_bin(104, 107, 1)
packer.pack()

#print('bin count %s' % len(packer))
#print('rect count %s' % len(packer[0]))

l = []
w = l.append
w('<canvas id="it" width=1000 height=5000></canvas>')
w('<script>')
w('  var canvas = document.getElementById("it");')
w('  var ctx = canvas.getContext("2d");')
w('  ctx.fillStyle = "#00FF00";')
w('  ctx.strokeStyle = "#000000";')
w('  ctx.font = "16px Arial";')

xScale = 10
yScale = 10

rectCount = 0

yBase = 0
xBase = 0
binCount = 0
for bin in packer:
  # Draw bin outline first:
  print('bin width %s, height %s' % (bin.width, bin.height))
  w('  ctx.fillStyle = "#FF0000";')
  w('  ctx.strokeRect(%s, %s, %s, %s);' %
    (xBase,
     yBase,
     int(bin.width * xScale),
     int(bin.height * yScale)))
  for rect in bin:
    rectCount += 1
    if False:
      w('  ctx.fillRect(%s, %s, %s, %s);' %
        (int(rect.x * xScale + xBase),
         int(rect.y * yScale + yBase),
         int(rect.width * xScale),
         int(rect.height * yScale)))
    w('  ctx.strokeRect(%s, %s, %s, %s);' %
      (int(rect.x * xScale + xBase),
       int(rect.y * yScale + yBase),
       int(rect.width * xScale),
       int(rect.height * yScale)))
    w('  ctx.fillStyle = "#000000";')
    w('  ctx.fillText("%sx%s", %s, %s);' %
      (rect.width, rect.height, int(rect.x * xScale + 5 + xBase), int((rect.y + rect.height / 2.) * yScale + yBase + 8)))
    w('  ctx.fillStyle = "#FF0000";')
  print('adding bin height %s' % bin.height)
  yBase += yScale * bin.height + 20
  binCount += 1
  if False and binCount == 2:
    yBase += yScale * 33
  if binCount == 3:
    yBase = 0
    xBase = 350
  elif binCount == 6:
    yBase = 0
    xBase = 700

w('</script>')

print('Packed %s rectangles' % rectCount)

if rectCount != expectedRectCount:
  raise RuntimeError('only packed %d of %d rectangles' % (rectCount, expectedRectCount))

open('/x/tmp/rect.html', 'w').write('\n'.join(l))

