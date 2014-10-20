STRING Word := 'FRED' :STORED('Word');

R := RECORD

 STRING SoFar {MAXLENGTH(200)};

 STRING Rest {MAXLENGTH(200)};

 END;

Init := DATASET([{'',Word}],R);

R Pluck1(DATASET(R) infile) := FUNCTION

R TakeOne(R le, UNSIGNED1 c) := TRANSFORM

 SELF.SoFar := le.SoFar + le.Rest[c];

 SELF.Rest := le.Rest[..c-1]+le.Rest[c+1..]; 

// Boundary Conditions handled automatically

 END;

RETURN NORMALIZE(infile,LENGTH(LEFT.Rest),TakeOne(LEFT,COUNTER));

 END;

L := LOOP(Init,LENGTH(TRIM(Word)),Pluck1(ROWS(LEFT)));

OUTPUT(L);

